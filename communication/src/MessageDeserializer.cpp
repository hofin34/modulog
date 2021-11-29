//
// Created by martin on 15.11.21.
//

#include <iostream>
#include "../include/MessageDeserializer.h"

MessageDeserializer::MessageDeserializer(const std::string& toDeserialize) {
    nlohmann::json msgObj = nlohmann::json::parse(toDeserialize);
    if(msgObj["msgType"] == Message::MSG_TYPE::LOG_MSG){
        msgType_ = Message::MSG_TYPE::LOG_MSG;
        logMessage_ = std::make_shared<LogMessage>(msgObj["message"]);
    }else if(msgObj["msgType"] == Message::MSG_TYPE::CONTROL_MSG){
        msgType_ = Message::MSG_TYPE::CONTROL_MSG;
        controlMessage_ = std::make_shared<ControlMessage>(msgObj["message"]);
    }
}

std::shared_ptr<LogMessage> MessageDeserializer::getLogMessage() {
    return logMessage_;
}

std::shared_ptr<ControlMessage> MessageDeserializer::getControlMessage() {
    return controlMessage_;
}

Message::MSG_TYPE MessageDeserializer::getMsgType() {
    return msgType_;
}
