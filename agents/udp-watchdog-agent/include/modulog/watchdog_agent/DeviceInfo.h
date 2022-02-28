#pragma once

#include <string>

namespace modulog::watchdog_agent{
    class DeviceInfo {
    public:
    private:
        std::string name_;
        int lastMessageTime_ = 0;
    };
}

