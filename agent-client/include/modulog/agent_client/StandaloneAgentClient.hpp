#pragma once

#include <string>
#include <memory>
#include <thread>
#include <modulog/communication/MessageExchanger.hpp>
#include <modulog/agent_client/AgentClient.hpp>
#include <utility>

namespace modulog::agent_client{
    class StandaloneAgentClient : public AgentClient{
    public:
        StandaloneAgentClient(std::string agentName) : agentName_(std::move(agentName)){};

        void sendLog(const std::shared_ptr<communication::LogMessage> &logMessage) override;

        void sendControl(const std::shared_ptr<communication::ControlMessage> &controlMessage) override;

        void initClient() override;

        void exitConnection() override;

        std::string getSharedConfig() override;

        bool canLog() override;

        bool sleepFor(std::chrono::seconds sleepTime) override;


    private:
        bool canLog_ = true;
        std::string agentName_;
    };
}

