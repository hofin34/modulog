#pragma once
#include "LogMessage.h"
#include "ControlMessage.h"
#include "Message.h"

class MessageSerializer {
public:
    MessageSerializer(std::shared_ptr<LogMessage> logMessage);
    MessageSerializer(std::shared_ptr<ControlMessage> controlMessage);
    std::string serialize();

private:
    std::shared_ptr<Message> message_;
    Message::MSG_TYPE messageType_;
};