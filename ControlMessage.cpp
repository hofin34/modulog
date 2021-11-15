//
// Created by martin on 10.11.21.
//

#include "ControlMessage.h"

std::string ControlMessage::serialize() {
    return nlohmann::json{{"type", type_}, {"value", value_}}.dump();
}

void ControlMessage::setType(ControlMessage::CONTOL_MSG_TYPE type) {
    type_ = type;
}

ControlMessage::CONTOL_MSG_TYPE ControlMessage::getType() {
    return type_;
}
