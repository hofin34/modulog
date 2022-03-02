#pragma once

#include <string>
#include <memory>
#include <modulog/agent_client/AgentClient.hpp>

namespace modulog::watchdog_agent{
    class DeviceInfo {
    public:
        DeviceInfo(std::string name, std::shared_ptr<agent_client::AgentClient> agentClient) : name_(name), agentClient_(agentClient){};
        void setSentMessage(bool sent);
        bool getSentMessage();
        std::string getName();
        void setTimestamp(uint64_t timestamp);
        uint64_t getTimestamp();
        void nextInactive();
    private:
        int inactiveCounter_ = 0;
        bool alreadyRestarted_ = false;
        bool isBroken_ = false;

        std::string name_;
        int lastMessageTime_ = 0;
        bool sentMessage_ = false;
        uint64_t timestamp_ = 0;
        const int MAX_INACTIVE = 3;
        std::shared_ptr<agent_client::AgentClient> agentClient_;

    };
}

