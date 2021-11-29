//
// Created by martin on 10.11.21.
//

#include "../include/LogMessage.h"

#include <utility>


LogMessage::LogMessage(LogMessage::LOG_MSG_TYPE logMsgType, std::string  key, std::string  value)
                            : logType_(logMsgType), key_(std::move(key)), value_(std::move(value)){
    //constructor
    setTimestampNow();

}
LogMessage::LogMessage(std::string jsonInit) {
    setTimestampNow();
    nlohmann::json jsonObj = nlohmann::json::parse(jsonInit); // TODO if wrong json
    logType_ = jsonObj["logMsgType"];
    key_ = jsonObj["key"];
    value_ = jsonObj["value"];
}


std::string LogMessage::serialize() {
    return nlohmann::json{{"logMsgType", logType_}, {"key", key_}, {"value", value_}}.dump();
}

std::string LogMessage::getValue() {
    return value_;
}

std::string LogMessage::getKey() {
    return key_;
}
