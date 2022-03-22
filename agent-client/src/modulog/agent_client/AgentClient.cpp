#include <modulog/agent_client/AgentClient.hpp>


namespace modulog::agent_client {

    AgentClient::AgentClient(std::shared_ptr<asio::io_context> &ioContext, std::string agentName)
            : ioContext_(ioContext), agentName_(agentName) {
        bringauto::logging::Logger::addSink<bringauto::logging::ConsoleSink>();
        bringauto::logging::Logger::LoggerSettings params{agentName_,
                                                          bringauto::logging::Logger::Verbosity::Debug};
        bringauto::logging::Logger::init(params);
#ifdef AGENT_CLIENT_DEBUG
        bringauto::logging::Logger::logWarning("AGENT_CLIENT_DEBUG macro is ON!");
#endif
        shouldExit_ = false;
        msgProcessor_ = std::make_shared<communication::MessageProcessor>(totalMsgsReceived_, msgCondVar_, msgMutex_);
    }

    AgentClient::~AgentClient() {
#ifndef AGENT_CLIENT_DEBUG
        responseHandleThread_.join();
        messageExchanger_->getConnection()->closeConnection();
        ioContext_->stop();
        clientThread_.join();
#endif
    }

    void AgentClient::signalHandler(int signum) {
        // Do nothing, just reproc++ library sends sigterm to all agents, when Core is interrupted with sigterm
        // - we want to stop agents on our own
    }

    void AgentClient::initClient() {


        struct sigaction sigAct{};
        memset(&sigAct, 0, sizeof(sigAct));
        sigAct.sa_handler = signalHandler;
        sigaction(SIGINT, &sigAct, nullptr);
        sigaction(SIGTERM, &sigAct, nullptr);


#ifdef AGENT_CLIENT_DEBUG
        return;
#endif
        auto connection = std::make_shared<communication::TcpConnection>(*ioContext_, agentName_, msgProcessor_);
        asio::ip::tcp::endpoint endpoint(asio::ip::address::from_string(coreIp_), corePort_);
        connection->getSocket().connect(endpoint);
        messageExchanger_ = std::make_shared<communication::MessageExchanger>(connection);
        clientThread_ = std::thread{[this]() { ioContext_->run(); }};
        auto configMessage = messageExchanger_->waitForControlMessage(
                2000); // If small timeout (like 2 ms), app crashes in Valgrind
        if (configMessage == nullptr)
            throw std::runtime_error("Not received config!");
        bringauto::logging::Logger::logInfo("Received config: {}", configMessage->getValue());
        sharedConfig_ = configMessage->getValue();
        auto ackMessage = std::make_shared<communication::ControlMessage>(
                modulog::communication::ControlMessage::CONTROL_MSG_TYPE::ACK, agentName_);
        messageExchanger_->sendControl(ackMessage);
        auto canStartSending = messageExchanger_->waitForControlMessage(-1);
        if (canStartSending == nullptr)
            throw std::runtime_error("Not received can start sending logs!");
        if (canStartSending->getType() != communication::ControlMessage::CONTROL_MSG_TYPE::ACK)
            throw std::runtime_error("Not received ACK for start sending logs!");
        responseHandleThread_ = std::thread{[this]() { handleResponses(); }};
    }


    void AgentClient::handleResponses() {
        while (!shouldExit_.load()) {
            auto controlMsg = messageExchanger_->waitForControlMessage(-1);
            if (controlMsg == nullptr) { // This should not happen
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


    void AgentClient::exitConnection() {
        auto exitMsg = std::make_shared<communication::ControlMessage>(
                modulog::communication::ControlMessage::CONTROL_MSG_TYPE::EXIT, "");
        sendControl(exitMsg);
        while (!shouldExit_.load()) { // Active waiting, but just a few millis, so its ok
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }
    }

    void AgentClient::sendLog(const std::shared_ptr<communication::LogMessage> &logMessage) {
#ifdef AGENT_CLIENT_DEBUG
        bringauto::logging::Logger::logInfo("Simulated log send (just debug): {}", logMessage->serialize());
#else
        messageExchanger_->sendLog(logMessage);
#endif
    }

    void AgentClient::sendControl(const std::shared_ptr<communication::ControlMessage> &controlMessage) {
#ifdef AGENT_CLIENT_DEBUG
        bringauto::logging::Logger::logInfo("Simulated log send (just debug): {}", controlMessage->serialize());
#else
        messageExchanger_->sendControl(controlMessage);
#endif
    }

    std::string AgentClient::getSharedConfig() {
        return sharedConfig_;
    }

    bool AgentClient::canLog() {
        return !shouldExit_;
    }

    bool AgentClient::sleepFor(const std::chrono::seconds sleepTime) {
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
