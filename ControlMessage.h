#pragma once

#include <string>
#include "Message.h"

class ControlMessage : public Message{
public:
    enum CONTOL_MSG_TYPE{CONFIG, ACK, IS_ALIVE};
    ControlMessage(std::string jsonInit);
    ControlMessage(CONTOL_MSG_TYPE msgType, std::string value);
    CONTOL_MSG_TYPE getType();
    void setType(CONTOL_MSG_TYPE type);
    void setValue(std::string value);
    std::string serialize() override;
private:
    CONTOL_MSG_TYPE type_;
    std::string value_;
    ControlMessage deserialize(std::string toDeserialize);
};

