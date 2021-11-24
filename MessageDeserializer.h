#pragma once
#include <string>
#include <memory>
#include "ControlMessage.h"
#include "LogMessage.h"
#include <nlohmann/json.hpp>

class MessageDeserializer {
public:
    MessageDeserializer(const std::string& toDeserialize);
    Message::MSG_TYPE getMsgType();
    std::shared_ptr<ControlMessage> getControlMessage();
    std::shared_ptr<LogMessage> getLogMessage();

private:
    Message::MSG_TYPE msgType_;
    std::shared_ptr<ControlMessage> controlMessage_;
    std::shared_ptr<LogMessage> logMessage_;
};
