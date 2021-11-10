//
// Created by martin on 10.11.21.
//

#include "MessageHandler.h"

MessageHandler::MessageHandler() {}
MessageHandler::MessageHandler(std::string toDeserialize) {
    deserialize(toDeserialize);
}
ControlMessage MessageHandler::getControlMessage() {
    return controlMessage;
}

LogMessage MessageHandler::getLogMessage() {
    return logMessage;
}

void MessageHandler::setControlMessage(ControlMessage msg) {
    controlMessage = msg;
    messageType = msg_type::CONTROL;
}

MessageHandler::msg_type MessageHandler::getMessageType() {
    return messageType;
}

void MessageHandler::setLogMessage(LogMessage msg) {
    logMessage = msg;
    messageType = msg_type::LOG;
}

std::string MessageHandler::serialize() {
    msg_type msgType = getMessageType();
    std::string msgContent;
    switch(msgType){
        case msg_type::LOG:
            msgContent = getLogMessage().serialize();
            break;
        case msg_type::CONTROL:
            msgContent = getControlMessage().serialize();
            break;
        default:
            msgContent = nlohmann::json{{"error", "this error should not happen."}}.dump();
    }
    return (nlohmann::json{{"msgType", msgType}, {"message", msgContent}}.dump() + "\n");

}