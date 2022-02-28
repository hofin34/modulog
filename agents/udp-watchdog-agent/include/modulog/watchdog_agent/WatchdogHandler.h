#pragma once

#include <string>
#include <iostream>
#include <nlohmann/json.hpp>
#include <modulog/watchdog_agent/DeviceInfo.h>

namespace modulog::watchdog_agent {
    class WatchdogHandler {
    public:
        void processMessage(const std::string &message);
        void init(std::vector<std::string> devices);
    private:
        std::vector<DeviceInfo> deviceInfoVector_;
    };
}