#pragma once
#include <string>
#include <memory>
#include "ControlMessage.h"
#include "LogMessage.h"

class MessageDeserializer {
public:
    MessageDeserializer(std::string toDeserialize);
    Message::MSG_TYPE getMsgType();
    std::shared_ptr<ControlMessage> getControlMessage();
    std::shared_ptr<LogMessage> getLogMessage();

private:
    Message::MSG_TYPE msgType_;
    std::shared_ptr<ControlMessage> controlMessage_;
    std::shared_ptr<LogMessage> logMessage_;
};
