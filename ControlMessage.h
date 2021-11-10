#pragma once

#include <string>
#include "Message.h"

class ControlMessage : public Message{
public:
    enum control_msg_t{CONFIG, ACK, IS_ALIVE};
    control_msg_t getType();
    void setType(control_msg_t type);
    void setValue(std::string value);
    std::string serialize() override;
private:
    control_msg_t type_;
    std::string value_;
    ControlMessage deserialize(std::string toDeserialize);
};

