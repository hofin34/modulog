#include <modulog/communication/ControlMessage.hpp>

namespace modulog::communication{
    ControlMessage::ControlMessage(std::string jsonInit) {
        setTimestampNow();
        nlohmann::json jsonObj = nlohmann::json::parse(jsonInit);
        type_ = jsonObj["controlMsgType"];
        value_ = jsonObj["value"];

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

}




