#pragma once

#include <string>
#include <filesystem>
#include <utility>
#include "reproc++/reproc.hpp"

class AgentInfo {
public:
    AgentInfo(const std::string &agentId, const std::filesystem::path &agentPath, const std::shared_ptr<reproc::process>& agentProcess,
              const std::shared_ptr<reproc::options> &agentProcessOptions);

    void setAgentId(std::string id);
    std::string getAgentId();
    std::shared_ptr<reproc::process> getAgentProcess();
    void stopAgent();
private:
    std::string agentId_;
    std::filesystem::path agentPath_;
    std::shared_ptr<reproc::process> agentProcess_;
    std::shared_ptr<reproc::options> agentProcessOptions_;
};