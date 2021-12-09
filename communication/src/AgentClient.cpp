
#include <iostream>
#include <filesystem>
#include <fstream>
#include "../include/AgentClient.h"
#include "../include/TcpConnection.h"
#include "../include/MessageDeserializer.h"

std::string AgentClient::getAgentConfig() {
    return agentConfig_;
}

AgentClient::AgentClient(std::shared_ptr<asio::io_context> &ioContext, bool isDebug, std::string agentName)
        : ioContext_(ioContext), isDebug_(isDebug), agentName_(agentName){
    msgProcessor_ = std::make_shared<MessageProcessor>(totalMsgsReceived_, msgCondVar_, msgMutex_);
}

void AgentClient::initClient() {
    if(isDebug_)
        return;
    try{
        //std::this_thread::sleep_for(std::chrono::seconds(15)); //TODO delete
        auto connection = TcpConnection::create(*ioContext_, agentName_, msgProcessor_);
        asio::ip::tcp::endpoint endpoint(asio::ip::address::from_string("127.0.0.1"), 1234);
        connection->get_socket().connect(endpoint);
        messageExchanger_ = std::make_shared<MessageExchanger>(connection);
        clientThread = std::thread{[this]() { ioContext_->run(); }};
        //auto configMessage = connection->getMessageProcessor_()->waitForControlMessage();//
        auto configMessage = messageExchanger_->waitForControlMessage();
        std::cout << agentName_ << " received config: " << configMessage->getValue() << std::endl;
        auto ackMessage = std::make_shared<ControlMessage>(ControlMessage::CONTROL_MSG_TYPE::ACK, agentName_);
        messageExchanger_->sendControl(ackMessage);
        auto canStartSending = messageExchanger_->waitForControlMessage();
        if(canStartSending->getType() != ControlMessage::CONTROL_MSG_TYPE::ACK){
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
    while(true){
        auto controlMsg = messageExchanger_->waitForControlMessage();
        if(controlMsg->getType() == ControlMessage::CONTROL_MSG_TYPE::IS_ALIVE){
            std::cerr << "Responding to IS_ALIVE!" << std::endl;
            auto ackAliveMsg = std::make_shared<ControlMessage>(ControlMessage::CONTROL_MSG_TYPE::ACK, "");
            messageExchanger_->sendControl(ackAliveMsg);
        }else if(controlMsg->getType() == ControlMessage::CONTROL_MSG_TYPE::EXIT_ACK){
            exitConfirmed.store(true);
        }
    }
}

std::string AgentClient::execCommand(const std::string& cmd) {
    std::array<char, 128> buffer;
    std::string result;
    std::unique_ptr<FILE, decltype(&pclose)> pipe(popen(cmd.c_str(), "r"), pclose);
    if (!pipe) {
        throw std::runtime_error("popen() failed!");
    }
    while (fgets(buffer.data(), buffer.size(), pipe.get()) != nullptr) {
        result += buffer.data();
    }
    return result;
}

nlohmann::json AgentClient::parseConfig(const std::string &execPath) {
    std::filesystem::path agentPath = execPath;
    agentPath = agentPath.remove_filename();
    std::ifstream ifs(agentPath/"config.json5");
    nlohmann::json configJson = nlohmann::json::parse(ifs);
    return configJson;
}

void AgentClient::exitConnection() {
    auto exitMsg = std::make_shared<ControlMessage>(ControlMessage::CONTROL_MSG_TYPE::EXIT, "");
    sendControl(exitMsg);
    while(!exitConfirmed.load()){ // Waits until Core didn't responded with exit ack
        std::cerr << "waiting..." << std::endl;
       // std::this_thread::sleep_for(std::chrono::milliseconds(400));
    }
    std::cerr <<"EXITED!" << std::endl;
}

void AgentClient::sendLog(const std::shared_ptr<LogMessage> &logMessage) {
    messageExchanger_->sendLog(logMessage);
}

void AgentClient::sendControl(const std::shared_ptr<ControlMessage> &controlMessage) {
    messageExchanger_->sendControl(controlMessage);
}


