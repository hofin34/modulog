#pragma once
#include "Message.h"
class LogMessage : public Message{
public:
    enum class LOG_MSG_TYPE{LOG, ERROR, DEBUG};
    LogMessage(std::string jsonInit);
    LogMessage(LOG_MSG_TYPE logMsgType, std::string  key, std::string  value);
    static std::string getTypeStr(LOG_MSG_TYPE logMsgType);
    LOG_MSG_TYPE getType();
    std::string serialize() override;
    std::string getKey();
    std::string getValue();
private:
    std::string key_;
    std::string value_;
    LOG_MSG_TYPE logType_;

};


