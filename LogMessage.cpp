//
// Created by martin on 10.11.21.
//

#include "LogMessage.h"


LogMessage::LogMessage(LogMessage::LOG_MSG_TYPE logMsgType, std::string value) {
    logType_ = logMsgType;
    value_ = value;
}
LogMessage::LogMessage(std::string jsonInit) {
    nlohmann::json jsonObj = nlohmann::json::parse(jsonInit);
    logType_ = jsonObj["logMsgType"];
    value_ = jsonObj["value"];
}


std::string LogMessage::serialize() {
    return nlohmann::json{{"logMsgType", logType_}, {"value", value_}}.dump();
}

std::string LogMessage::getValue() {
    return value_;
}
