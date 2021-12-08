#pragma once

#include <string>
#include "Message.h"

class ControlMessage : public Message{
public:
    enum class CONTROL_MSG_TYPE{CONFIG, ACK, IS_ALIVE, EXIT};
    ControlMessage(std::string jsonInit);
    ControlMessage(CONTROL_MSG_TYPE msgType, std::string value);
    CONTROL_MSG_TYPE getType();
    std::string getValue();
    std::string serialize() override;
private:
    CONTROL_MSG_TYPE type_;
    std::string value_;
    ControlMessage deserialize(std::string toDeserialize);
};

