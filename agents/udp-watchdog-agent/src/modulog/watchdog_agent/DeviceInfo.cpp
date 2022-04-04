#include <modulog/watchdog_agent/DeviceInfo.hpp>

namespace modulog::watchdog_agent {

    bool DeviceInfo::getSentMessage() {
        return sentMessage_;
    }

    void DeviceInfo::setSentMessage(bool sent) {
        sentMessage_ = sent;
    }

    std::string DeviceInfo::getName() {
        return name_;
    }

    uint64_t DeviceInfo::getTimestamp() {
        return timestamp_;
    }

    void DeviceInfo::setTimestamp(uint64_t timestamp) {
        timestamp_ = timestamp;
    }

    void DeviceInfo::nextInactive() {
        inactiveCounter_++;
        if (isBroken_)
            return;
        else if (alreadyRestarted_) {
            isBroken_ = true;
            auto errorMsg = std::make_shared<communication::LogMessage>(communication::LogMessage::LOG_MSG_TYPE::ERROR,
                                                                        name_, "Not responding even after restart");
            agentClient_->sendLog(errorMsg);
        } else if (inactiveCounter_ >= MAX_INACTIVE) {
            auto warningMsg = std::make_shared<communication::LogMessage>(
                    communication::LogMessage::LOG_MSG_TYPE::WARNING, name_, "Not received alive msg");
            agentClient_->sendLog(warningMsg);
            alreadyRestarted_ = true;
        }
    }


}
