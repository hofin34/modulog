#include <modulog/communication/MessageDeserializer.hpp>

namespace modulog::communication{
    MessageDeserializer::MessageDeserializer(const std::string& toDeserialize) {
        nlohmann::json msgObj = nlohmann::json::parse(toDeserialize);
        if(msgObj["msgType"] == MSG_TYPE::LOG_MSG){
            msgType_ = MSG_TYPE::LOG_MSG;
            logMessage_ = std::make_shared<LogMessage>(msgObj["message"]);
        }else if(msgObj["msgType"] == MSG_TYPE::CONTROL_MSG){
            msgType_ = MSG_TYPE::CONTROL_MSG;
            controlMessage_ = std::make_shared<ControlMessage>(msgObj["message"]);
        }
    }

    std::shared_ptr<LogMessage> MessageDeserializer::getLogMessage() {
        return logMessage_;
    }

    std::shared_ptr<ControlMessage> MessageDeserializer::getControlMessage() {
        return controlMessage_;
    }

    MSG_TYPE MessageDeserializer::getMsgType() {
        return msgType_;
    }
}

