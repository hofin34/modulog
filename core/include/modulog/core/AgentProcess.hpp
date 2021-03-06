#pragma once

#include <reproc++/reproc.hpp>

#include <string>
#include <filesystem>
#include <iostream>

namespace modulog::core{
/**
 * This class represents running instance of agent
 */
    class AgentProcess {
    public:
        /**
         * @param agentId can be set later by setAgentId
         * @param agentPath path of executable
         * @param agentProcess process representing running agent
         * @param agentProcessOptions options with defined stop argument - used to send SIGTERM first and after time SIGKILL
         */
        AgentProcess(std::string agentId, std::filesystem::path agentPath,
                     std::shared_ptr<reproc::process> agentProcess,
                     std::shared_ptr<reproc::options> agentProcessOptions);

        /**
         * Setter
         * @param id id to be set (mainly use just for debug identification)
         */
        void setAgentId(std::string id);

        std::string getAgentId();

        /**
         * Stop agent process (first sends SIGTERM and then SIGKILL)
         */
        void stopAgent();

    private:
        std::string agentId_;
        std::filesystem::path agentPath_;
        std::shared_ptr<reproc::process> agentProcess_;
        std::shared_ptr<reproc::options> agentProcessOptions_;
    };
}
