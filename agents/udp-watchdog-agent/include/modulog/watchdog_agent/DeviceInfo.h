#pragma once

#include <string>

namespace modulog::watchdog_agent{
    class DeviceInfo {
    public:
        DeviceInfo(std::string name) : name_(name){};
        void setSentMessage(bool sent);
        bool getSentMessage();
        std::string getName();
        void setTimestamp(uint64_t timestamp);
        uint64_t getTimestamp();
    private:
        std::string name_;
        int lastMessageTime_ = 0;
        bool sentMessage_ = false;
        uint64_t timestamp_ = 0;
    };
}

