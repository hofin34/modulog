//
// Created by martin on 15.11.21.
//

#include "../include/MessageSerializer.hpp"

MessageSerializer::MessageSerializer(std::shared_ptr<LogMessage> logMessage) {
    message_ = std::dynamic_pointer_cast<Message>(logMessage);
    messageType_ = Message::MSG_TYPE::LOG_MSG;
}

MessageSerializer::MessageSerializer(std::shared_ptr<ControlMessage> controlMessage) {
    message_ = std::dynamic_pointer_cast<Message>(controlMessage);
    messageType_ = Message::MSG_TYPE::CONTROL_MSG;
}

std::string MessageSerializer::serialize() {
    return (nlohmann::json{{"msgType", messageType_}, {"message", message_->serialize()}}.dump());
}
