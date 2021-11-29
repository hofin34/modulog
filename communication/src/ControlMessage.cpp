//
// Created by martin on 10.11.21.
//

#include "../include/ControlMessage.h"
ControlMessage::ControlMessage(std::string jsonInit) {
    setTimestampNow();
    nlohmann::json jsonObj = nlohmann::json::parse(jsonInit);
    type_ = jsonObj["controlMsgType"];
    value_ = jsonObj["value"]; // TODO check if correct type

}
ControlMessage::ControlMessage(ControlMessage::CONTROL_MSG_TYPE msgType, std::string value) {
    setTimestampNow();
    type_ = msgType;
    value_ = value;
}

std::string ControlMessage::serialize() {
    return nlohmann::json{{"controlMsgType", type_}, {"value", value_}}.dump();
}

ControlMessage::CONTROL_MSG_TYPE ControlMessage::getType() {
    return type_;
}

std::string ControlMessage::getValue() {
    return value_;
}




