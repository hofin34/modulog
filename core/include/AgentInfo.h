#pragma once

#include <string>
#include <filesystem>
#include <utility>
#include "reproc++/reproc.hpp"

class AgentInfo {
public:
    AgentInfo(std::string agentId, std::filesystem::path agentPath, std::shared_ptr<reproc::process> agentProcess_):
            agentId_(std::move(agentId)), agentPath_(std::move(agentPath)), agentProcess_(std::move(agentProcess_)){}
    void setAgentId(std::string id);
private:
    std::string agentId_;
    std::filesystem::path agentPath_;
    std::shared_ptr<reproc::process> agentProcess_;
};