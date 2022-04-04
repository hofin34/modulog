#pragma once

#include <modulog/watchdog_agent/DeviceInfo.hpp>
#include <modulog/agent_client/AgentClient.hpp>

#include <nlohmann/json.hpp>
#include <asio.hpp>

#include <string>
#include <iostream>
#include <regex>


namespace modulog::watchdog_agent {
    /**
     * Class used for checking, if all devices are responding correctly
     */
    class WatchdogHandler {
    public:
        /**
         *
         * @param ioContext
         * @param checkoutInterval how often (in seconds) check, if agents sent alive message
         * @param agentClient initialized instance of AgentClient - used for sending logs
         */
        WatchdogHandler(std::shared_ptr<asio::io_context> &ioContext, int checkoutInterval,
                        std::shared_ptr<agent_client::AgentClient> agentClient) : checkoutInterval_(checkoutInterval),
                                                                                  checkoutTimer_(*ioContext),
                                                                                  agentClient_(agentClient) {};

        /**
         * Process received message - marking, that agent responded
         * @param message
         */
        void processMessage(const std::string &message);

        /**
         * Init watchdog handler - init timer etc.
         * @param devices vector of devices names, that are expected to send alive messages
         */
        void init(std::vector<std::string> devices);

    private:
        /**
         * Check, if all devices sent alive messages. If not, log it.
         */
        void checkAllDevices(const asio::error_code &);

        std::vector<DeviceInfo> deviceInfoVector_;
        std::shared_ptr<agent_client::AgentClient> agentClient_;
        asio::steady_timer checkoutTimer_;
        int checkoutInterval_ = 10;
        std::mutex mtx_;
    };
}