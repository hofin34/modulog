#include <modulog/watchdog_agent/DeviceInfo.h>

namespace modulog::watchdog_agent{

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
}
