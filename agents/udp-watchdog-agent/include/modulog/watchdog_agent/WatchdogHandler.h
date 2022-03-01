#pragma once

#include <modulog/watchdog_agent/DeviceInfo.h>

#include <nlohmann/json.hpp>
#include <asio.hpp>

#include <string>
#include <iostream>
#include <regex>

namespace modulog::watchdog_agent {
    class WatchdogHandler {
    public:
        WatchdogHandler(asio::io_context &ioContext) : checkoutTimer(ioContext, std::chrono::seconds(checkoutTime)) {};

        void processMessage(const std::string &message);

        void init(std::vector<std::string> devices);

    private:
        void checkAllDevices(const asio::error_code &);

        std::vector<DeviceInfo> deviceInfoVector_;
        asio::steady_timer checkoutTimer;
        int checkoutTime = 10;
        std::mutex mtx_;
    };
}