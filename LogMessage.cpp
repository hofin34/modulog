//
// Created by martin on 10.11.21.
//

#include "LogMessage.h"


LogMessage::LogMessage(LogMessage::LOG_MSG_TYPE logMsgType, std::string value) {
    logType_ = logMsgType;
    value_ = value;
}
LogMessage::LogMessage(std::string jsonInit) {
    //TODO init this class depending on json
}


std::string LogMessage::serialize() {
    return nlohmann::json{{"type", logType_}, {"value", value_}}.dump();
}

LogMessage LogMessage::deserialize(std::string toDeserialize) {
    nlohmann::json jsonObj = nlohmann::json::parse(toDeserialize);
    //TODO
}
