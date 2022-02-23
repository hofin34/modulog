#include <modulog/agent_client/AgentClient.hpp>


namespace modulog::agent_client{

    AgentClient::AgentClient(std::shared_ptr<asio::io_context> &ioContext, bool isDebug, std::string agentName)
            : ioContext_(ioContext), isDebug_(isDebug), agentName_(agentName){
        confirmedExit_ = false;
        msgProcessor_ = std::make_shared<communication::MessageProcessor>(totalMsgsReceived_, msgCondVar_, msgMutex_);
    }

    void AgentClient::signalHandler(int signum) {
        // Do nothing, just reproc++ library sends sigterm to all agents, when Core is interrupted with sigterm
        // - we want to stop agents on our own
    }

    void AgentClient::initClient() {
        struct sigaction sigAct{};
        memset(&sigAct, 0, sizeof(sigAct));
        sigAct.sa_handler = signalHandler;
        sigaction(SIGINT,  &sigAct, nullptr);
        sigaction(SIGTERM, &sigAct, nullptr);

        if(isDebug_)
            return;
        try{
            auto connection = std::make_shared<communication::TcpConnection>(*ioContext_, agentName_, msgProcessor_);
            asio::ip::tcp::endpoint endpoint(asio::ip::address::from_string("127.0.0.1"), 1234);
            connection->getSocket().connect(endpoint);
            messageExchanger_ = std::make_shared<communication::MessageExchanger>(connection);
            clientThread = std::thread{[this]() { ioContext_->run(); }};
            auto configMessage = messageExchanger_->waitForControlMessage(2000); //TODO if timeout 2 ms and run with valgrind, error occur
            if(configMessage == nullptr){
                bringauto::logging::Logger::logError("{} didnt receive configMessage", agentName_);
                exit(EXIT_FAILURE);
            }
            std::cout << agentName_ << " received config: " << configMessage->getValue() << std::endl;
            sharedConfig = configMessage->getValue();
            auto ackMessage = std::make_shared<communication::ControlMessage>(modulog::communication::ControlMessage::CONTROL_MSG_TYPE::ACK, agentName_);
            messageExchanger_->sendControl(ackMessage);
            auto canStartSending = messageExchanger_->waitForControlMessage(-1);
            if(canStartSending == nullptr){
                std::cerr << agentName_ << " didnt receive can start sending logs! " << std::endl;
                exit(EXIT_FAILURE);
            }
            if(canStartSending->getType() != communication::ControlMessage::CONTROL_MSG_TYPE::ACK){
                std::cerr << "Not received ACK for start sending logs.";
                exit(EXIT_FAILURE);
            }
            responseHandleThread = std::thread{[this](){ handleResponses(); }};

        }catch(std::exception& e) {
            std::cerr << e.what() << std::endl;
            exit(EXIT_FAILURE);
        }catch(...){
            std::cerr << "Error in AgentClient init..." << std::endl;
            exit(EXIT_FAILURE);
        }
    }



    void AgentClient::handleResponses() {
        while(confirmedExit_.load() != true){
            auto controlMsg = messageExchanger_->waitForControlMessage(-1);
            if(controlMsg == nullptr){ // This should not happen
                std::cerr << "No control message!"<< std::endl;
                continue;
            }
            if(controlMsg->getType() == communication::ControlMessage::CONTROL_MSG_TYPE::IS_ALIVE){
                if(simulatedFreeze.load()) // for testing purposes, you can simulate, that client doesn't respond to IS_ALIVE messages
                    continue;
                auto ackAliveMsg = std::make_shared<communication::ControlMessage>(modulog::communication::ControlMessage::CONTROL_MSG_TYPE::ACK, "");
                messageExchanger_->sendControl(ackAliveMsg);
            }else if(controlMsg->getType() == communication::ControlMessage::CONTROL_MSG_TYPE::EXIT){
                std::cout << "Agent received EXIT - should exit now." << std::endl; //TODO exit
            }else if(controlMsg->getType() == communication::ControlMessage::CONTROL_MSG_TYPE::EXIT_ACK){
                confirmedExit_ = true;
            }
        }
    }





    void AgentClient::exitConnection() {
        auto exitMsg = std::make_shared<communication::ControlMessage>(modulog::communication::ControlMessage::CONTROL_MSG_TYPE::EXIT, "");
        sendControl(exitMsg);
        while(confirmedExit_.load() != true){ // Active waiting, but just a few millis, so its ok
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }
        responseHandleThread.join();
        messageExchanger_->getConnection()->closeConnection();
    }

    void AgentClient::sendLog(const std::shared_ptr<communication::LogMessage> &logMessage) {
        if(isDebug_){
            std::cout << "Simulated log send (just debug): " << logMessage->serialize() << std::endl;
        }else{
            messageExchanger_->sendLog(logMessage);
        }
    }

    void AgentClient::sendControl(const std::shared_ptr<communication::ControlMessage> &controlMessage) {
        if (isDebug_) {
            std::cout << "Simulated control send (just debug): " << controlMessage->serialize() << std::endl;
        } else {
            messageExchanger_->sendControl(controlMessage);
        }
    }

    std::string AgentClient::getSharedConfig() {
        return sharedConfig;
    }

    void AgentClient::freezeClient() {
        simulatedFreeze = true;
    }


}
