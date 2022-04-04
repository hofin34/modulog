#include <modulog/agent_client/TcpAgentClient.hpp>


namespace modulog::agent_client {
    TcpAgentClient::TcpAgentClient(std::shared_ptr<asio::io_context> &ioContext, std::string agentName)
            : ioContext_(ioContext), agentName_(std::move(agentName)) {
        shouldExit_ = false;
        msgProcessor_ = std::make_shared<communication::MessageProcessor>(totalMsgsReceived_, msgCondVar_, msgMutex_);
    }

    TcpAgentClient::~TcpAgentClient() {
        responseHandleThread_.join();
        messageExchanger_->getConnection()->closeConnection();
        ioContext_->stop();
        clientThread_.join();
    }

    void TcpAgentClient::signalHandler(const std::error_code &error,
                                       int signum) {
        // reproc++ sending kill signal to all subprocesses, we want to just catch it and do nothing.
        // Core will kill all subprocesses on its own
        bringauto::logging::Logger::logDebug("Cought signal no. {}", signum);
    }

    void TcpAgentClient::initClient() {
        generalInit(agentName_);

        interruptSignals = std::make_shared<asio::signal_set>(*ioContext_, SIGINT, SIGTERM);
        interruptSignals->async_wait(signalHandler);

        auto connection = std::make_shared<communication::TcpConnection>(*ioContext_, agentName_, msgProcessor_);
        asio::ip::tcp::endpoint endpoint(asio::ip::address::from_string(coreIp_), corePort_);
        connection->getSocket().connect(endpoint);
        messageExchanger_ = std::make_shared<communication::MessageExchanger>(connection);
        clientThread_ = std::thread{[this]() { ioContext_->run(); }};
        constexpr int CONTROL_MSG_WAIT = 2000;
        auto configMessage = messageExchanger_->waitForControlMessage(
                CONTROL_MSG_WAIT); // If small timeout (like 2 ms), app crashes in Valgrind
        if (!configMessage)
            throw std::runtime_error("Not received config!");
        bringauto::logging::Logger::logInfo("Received config: {}", configMessage->getValue());
        sharedConfig_ = configMessage->getValue();
        auto ackMessage = std::make_shared<communication::ControlMessage>(
                modulog::communication::ControlMessage::CONTROL_MSG_TYPE::ACK, agentName_);
        messageExchanger_->sendControl(ackMessage);
        auto canStartSending = messageExchanger_->waitForControlMessage(
                communication::MessageExchanger::INFINITE_TIMEOUT);
        if (canStartSending == nullptr)
            throw std::runtime_error("Not received can start sending logs!");
        if (canStartSending->getType() != communication::ControlMessage::CONTROL_MSG_TYPE::ACK)
            throw std::runtime_error("Not received ACK for start sending logs!");
        responseHandleThread_ = std::thread{[this]() { handleResponses(); }};
    }


    void TcpAgentClient::handleResponses() {
        while (!shouldExit_.load()) {
            auto controlMsg = messageExchanger_->waitForControlMessage(
                    communication::MessageExchanger::INFINITE_TIMEOUT);
            if (!controlMsg) { // This should not happen
                bringauto::logging::Logger::logError("No control message!");
                continue;
            }
            if (controlMsg->getType() == communication::ControlMessage::CONTROL_MSG_TYPE::IS_ALIVE) {
                auto ackAliveMsg = std::make_shared<communication::ControlMessage>(
                        modulog::communication::ControlMessage::CONTROL_MSG_TYPE::ACK, "");
                messageExchanger_->sendControl(ackAliveMsg);
            } else if (controlMsg->getType() == communication::ControlMessage::CONTROL_MSG_TYPE::EXIT) {
                {
                    std::lock_guard<std::mutex> lock(waitMutex_);
                    waitEnd_ = true;
                }
                waitCondVar_.notify_all();
                shouldExit_ = true;
            }
        }
    }


    void TcpAgentClient::exitConnection() {
        auto exitMsg = std::make_shared<communication::ControlMessage>(
                modulog::communication::ControlMessage::CONTROL_MSG_TYPE::EXIT, "");
        sendControl(exitMsg);
        while (!shouldExit_.load()) { // Active waiting, but just a few millis, so its ok
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }
    }

    void TcpAgentClient::sendLog(const std::shared_ptr<communication::LogMessage> &logMessage) {
        messageExchanger_->sendLog(logMessage);
    }

    void TcpAgentClient::sendControl(const std::shared_ptr<communication::ControlMessage> &controlMessage) {
        messageExchanger_->sendControl(controlMessage);
    }

    std::string TcpAgentClient::getSharedConfig() {
        return sharedConfig_;
    }

    bool TcpAgentClient::canLog() {
        return !shouldExit_;
    }

    bool TcpAgentClient::sleepFor(const std::chrono::seconds sleepTime) {
        const auto time_point = std::chrono::system_clock::now() + sleepTime;
        while (!waitEnd_) {
            std::unique_lock<std::mutex> lock(waitMutex_);
            const std::cv_status status = waitCondVar_.wait_until(lock, time_point);
            if (status == std::cv_status::timeout) {
                return true;
            } else { // no timeout
                if (waitEnd_) // else is spurious wakeup
                    return false;
            }
        }
        return true;
    }

}
