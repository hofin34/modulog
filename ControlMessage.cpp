//
// Created by martin on 10.11.21.
//

#include "ControlMessage.h"

std::string ControlMessage::serialize() {
    return nlohmann::json{{"type", type_}, {"value", value_}}.dump();
}

void ControlMessage::setType(ControlMessage::control_msg_t type) {
    type_ = type;
}

ControlMessage::control_msg_t ControlMessage::getType() {
    return type_;
}
