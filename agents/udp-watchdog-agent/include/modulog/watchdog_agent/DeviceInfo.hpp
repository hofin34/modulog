#pragma once

#include <string>
#include <memory>
#include <modulog/agent_client/AgentClient.hpp>

namespace modulog::watchdog_agent {
    /**
     * Class representing real world device, that should send udp messages in some time interval telling, that it's alive.
     */
    class DeviceInfo {
    public:
        /**
         *
         * @param name How is device named (something like "BUTTON1")
         * @param agentClient initialized agent client - will send through it log messages
         */
        DeviceInfo(std::string name, std::shared_ptr<agent_client::AgentClient> agentClient) : name_(name),
                                                                                               agentClient_(
                                                                                                       agentClient) {};

        /**
         * Setter - mark, if device sent udp message marking, that he is alive
         * @param sent
         */
        void setSentMessage(bool sent);

        /**
         *
         * @return if device sent udp message marking, that he is alive
         */
        bool getSentMessage();

        /**
         * Get device name
         * @return device name
         */
        std::string getName();

        /**
         * As a part of alive message is timestamp saying, when was message created.
         * This timestamp is not used now, because there is no use case, it is just prepared for the future
         * @param timestamp
         */
        void setTimestamp(uint64_t timestamp);

        /**
         * Get timestamp of last udp alive message
         * @return time in seconds from device start
         */
        uint64_t getTimestamp();

        /**
         * Make some action, when device didn't send alive message
         */
        void nextInactive();

    private:
        int inactiveCounter_ = 0;
        bool alreadyRestarted_ = false;
        bool isBroken_ = false;

        std::string name_;
        int lastMessageTime_ = 0;
        bool sentMessage_ = false;
        uint64_t timestamp_ = 0;
        const int MAX_INACTIVE = 1;
        std::shared_ptr<agent_client::AgentClient> agentClient_;

    };
}

