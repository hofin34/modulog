//
// Created by martin on 10.11.21.
//

#include "ControlMessage.h"
ControlMessage::ControlMessage(std::string jsonInit) {
    //TODO

}
ControlMessage::ControlMessage(ControlMessage::CONTROL_MSG_TYPE msgType, std::string value) {
    type_ = msgType;
    value_ = value;
}

std::string ControlMessage::serialize() {
    return nlohmann::json{{"type", type_}, {"value", value_}}.dump();
}

ControlMessage::CONTROL_MSG_TYPE ControlMessage::getType() {
    return type_;
}




