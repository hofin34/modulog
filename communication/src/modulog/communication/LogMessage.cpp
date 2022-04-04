#include <modulog/communication/LogMessage.hpp>

#include <utility>

namespace modulog::communication{
    LogMessage::LogMessage(LogMessage::LOG_MSG_TYPE logMsgType, std::string  key, std::string  value)
            : logType_(logMsgType), key_(std::move(key)), value_(std::move(value)){
        //constructor
        setTimestampNow();

    }
    LogMessage::LogMessage(std::string jsonInit) {
        setTimestampNow();
        nlohmann::json jsonObj = nlohmann::json::parse(jsonInit);
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

    std::string LogMessage::getTypeStr(LogMessage::LOG_MSG_TYPE logMsgType) {
        switch (logMsgType) {
            case LOG_MSG_TYPE::DEBUG:
                return "DEBUG";
            case LOG_MSG_TYPE::ERROR:
                return "ERROR";
            case LOG_MSG_TYPE::LOG:
                return "LOG";
            case LOG_MSG_TYPE::WARNING:
                return "WARNING";
            default:
                return "UNKNOWN";
        }
    }

    LogMessage::LOG_MSG_TYPE LogMessage::getType() {
        return logType_;
    }

}
