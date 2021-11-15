#pragma once
#include "Message.h"
class LogMessage : public Message{
public:
    enum class LOG_MSG_TYPE{LOG, ERROR, DEBUG};
    LogMessage(std::string jsonInit);
    LogMessage(LOG_MSG_TYPE logMsgType, std::string value);
    LogMessage deserialize(std::string toDeserialize);
    std::string serialize() override;
private:
    std::string value_;
    LOG_MSG_TYPE logType_;

};


