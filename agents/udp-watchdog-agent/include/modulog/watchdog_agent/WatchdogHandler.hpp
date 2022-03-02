#pragma once

#include <modulog/watchdog_agent/DeviceInfo.hpp>
#include <modulog/agent_client/AgentClient.hpp>

#include <nlohmann/json.hpp>
#include <asio.hpp>

#include <string>
#include <iostream>
#include <regex>


namespace modulog::watchdog_agent {
    class WatchdogHandler {
    public:
        WatchdogHandler(std::shared_ptr<asio::io_context> &ioContext, int checkoutInterval, std::shared_ptr<agent_client::AgentClient> agentClient) : checkoutInterval_(checkoutInterval),
                                                                             checkoutTimer_(*ioContext,
                                                                                            std::chrono::seconds(
                                                                                                    checkoutInterval_)),
                                                                                                    agentClient_(agentClient){};

        void processMessage(const std::string &message);

        void init(std::vector<std::string> devices);

    private:
        void checkAllDevices(const asio::error_code &);

        std::vector<DeviceInfo> deviceInfoVector_;
        std::shared_ptr<agent_client::AgentClient> agentClient_;
        asio::steady_timer checkoutTimer_;
        int checkoutInterval_ = 10;
        std::mutex mtx_;
    };
}