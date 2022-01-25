#include <modulog/agent_client/AgentClient.hpp>


namespace modulog::agent_client{

    AgentClient::AgentClient(std::shared_ptr<asio::io_context> &ioContext, bool isDebug, std::string agentName)
            : ioContext_(ioContext), isDebug_(isDebug), agentName_(agentName){
        confirmedExit_ = false;
        msgProcessor_ = std::make_shared<communication::MessageProcessor>(totalMsgsReceived_, msgCondVar_, msgMutex_);
    }

    void AgentClient::initClient() {
        if(isDebug_)
            return;
        try{
            auto connection = communication::TcpConnection::create(*ioContext_, agentName_, msgProcessor_);
            asio::ip::tcp::endpoint endpoint(asio::ip::address::from_string("127.0.0.1"), 1234);
            connection->getSocket().connect(endpoint);
            messageExchanger_ = std::make_shared<communication::MessageExchanger>(connection);
            clientThread = std::thread{[this]() { ioContext_->run(); }};
            auto configMessage = messageExchanger_->waitForControlMessage(2000); //TODO if timeout 2 ms and run with valgrind, error occur
            if(configMessage == nullptr){
                std::cerr << agentName_ << " didnt receive configMessage!" << std::endl;
                exit(EXIT_FAILURE);
            }
            std::cout << agentName_ << " received config: " << configMessage->getValue() << std::endl;
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
            if(controlMsg == nullptr){
                std::cerr << "TODO shouldnt be"<< std::endl;
                continue;
            }
            if(controlMsg->getType() == communication::ControlMessage::CONTROL_MSG_TYPE::IS_ALIVE){
                std::cout << "Agent responding to IS_ALIVE!" << std::endl;
                auto ackAliveMsg = std::make_shared<communication::ControlMessage>(modulog::communication::ControlMessage::CONTROL_MSG_TYPE::ACK, "");
                messageExchanger_->sendControl(ackAliveMsg);
            }else if(controlMsg->getType() == communication::ControlMessage::CONTROL_MSG_TYPE::EXIT){
                std::cout << "Agent received EXIT - should exit now." << std::endl; //TODO exit
            }else if(controlMsg->getType() == communication::ControlMessage::CONTROL_MSG_TYPE::EXIT_ACK){
                std::cout << "Agent received EXIT_ACK - can exit now." << std::endl; //TODO
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
        messageExchanger_->getConnection()->closeConnection(); // TODO maybe remove?
    }

    void AgentClient::sendLog(const std::shared_ptr<communication::LogMessage> &logMessage) {
        if(isDebug_){
            std::cout << "Simulated send (not same as real send): " << logMessage->serialize() << std::endl;
        }else{
            messageExchanger_->sendLog(logMessage);
        }
    }

    void AgentClient::sendControl(const std::shared_ptr<communication::ControlMessage> &controlMessage) {
        if (isDebug_) {
            std::cout << "Simulated send (not same as real send): " << controlMessage->serialize() << std::endl;
        } else {
            messageExchanger_->sendControl(controlMessage);
        }
    }


}
