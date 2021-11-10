#pragma once

#include <string>
#include "ControlMessage.h"
#include "LogMessage.h"

class MessageHandler {
public:
    MessageHandler(std::string toDeserialize); // To deserialize
    MessageHandler();
    enum msg_type{LOG, CONTROL};
    void setControlMessage(ControlMessage msg);
    ControlMessage getControlMessage();
    void setLogMessage(LogMessage msg);
    LogMessage getLogMessage();
    msg_type getMessageType();
    std::string serialize();


private:
    void deserialize(std::string toDeserialize);
    msg_type messageType;
    ControlMessage controlMessage;
    LogMessage logMessage;
};
