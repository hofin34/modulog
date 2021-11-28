
#include <iostream>
#include "AgentClient.h"
#include "TcpConnection.h"
#include "MessageDeserializer.h"

std::string AgentClient::getAgentConfig() {
    return agentConfig_;
}

AgentClient::AgentClient(std::shared_ptr<asio::io_context> &ioContext, bool isDebug, std::string agentName)
        : ioContext_(ioContext), isDebug_(isDebug), agentName_(agentName){
    initClient();
}

void AgentClient::initClient() {
    if(isDebug_)
        return;
    try{
        connection_ = TcpConnection::create(*ioContext_, agentName_);
        asio::ip::tcp::endpoint endpoint(asio::ip::address::from_string("127.0.0.1"), 1234);
        connection_->get_socket().connect(endpoint);
        connection_->start_read();
        clientThread = std::thread{[this]() { ioContext_->run(); }};

        std::shared_ptr<std::string> configMsgString;
        while ((configMsgString = connection_->popMessage()) == nullptr);
        std::cout << agentName_ << " received config: " << *configMsgString << std::endl;
        // received config
        MessageDeserializer messageDeserializer(*configMsgString);
        if(messageDeserializer.getMsgType() == Message::MSG_TYPE::CONTROL_MSG){
            auto configMessage = messageDeserializer.getControlMessage();
            nlohmann::json configJson = nlohmann::json::parse(configMessage->getValue());
            agentConfig_ = configMessage->getValue();
            //TODO parse json
            std::string agentId = configJson["id"];
            agentName_ = agentId;
            auto ackMessage = std::make_shared<ControlMessage>(ControlMessage::CONTROL_MSG_TYPE::ACK, agentId);
            MessageSerializer msgSerializer(ackMessage);
            std::string initResponse = msgSerializer.serialize();
            connection_->send_message(initResponse);
        }
    }catch(std::exception& e) {
        std::cerr << e.what() << std::endl;
        exit(EXIT_FAILURE);
    }catch(...){
        std::cerr << "Error in AgentClient init..." << std::endl;
        exit(EXIT_FAILURE);
    }
}

void AgentClient::sendLog(const std::shared_ptr<LogMessage>& logMessage) {
    std::cout << "HERE" << std::endl;
    MessageSerializer messageSerializer(logMessage);
    std::string toSend = messageSerializer.serialize();
    if(isDebug_){
        std::cout << "Simulated send: " << toSend << std::endl;
    } else{
        connection_->send_message(toSend);
    }
}

void AgentClient::handleResponses() {
    std::shared_ptr<std::string> messageString;
    while ((messageString = connection_->popMessage()) == nullptr);
    MessageDeserializer messageDeserializer1(*messageString);
    if(messageDeserializer1.getMsgType() == Message::MSG_TYPE::CONTROL_MSG){
        std::shared_ptr<ControlMessage> controlMessage = messageDeserializer1.getControlMessage();
        if(controlMessage->getType() == ControlMessage::CONTROL_MSG_TYPE::IS_ALIVE){ // Response to isAlive
            auto ackAliveMsg = std::make_shared<ControlMessage>(ControlMessage::CONTROL_MSG_TYPE::ACK, "");
            MessageSerializer messageSerializer(ackAliveMsg);
            std::string toSend = messageSerializer.serialize();
            std::cout << agentName_ << " sending: " << toSend;
            connection_->send_message(toSend);
            std::cout << "sentX";
        }
    }
}


