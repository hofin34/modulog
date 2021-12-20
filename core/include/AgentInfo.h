#pragma once

#include <string>
#include <filesystem>
#include <utility>
#include "reproc++/reproc.hpp"

/**
 * This class represents running instance of agent
 */
class AgentInfo {
public:
    /**
     * @param agentId can be set later by setAgentId
     * @param agentPath path of executable
     * @param agentProcess process representing running agent
     * @param agentProcessOptions options with defined stop argument - used to send SIGTERM first and after time SIGKILL
     */
    AgentInfo(const std::string &agentId, const std::filesystem::path &agentPath,
              const std::shared_ptr<reproc::process> &agentProcess,
              const std::shared_ptr<reproc::options> &agentProcessOptions);

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