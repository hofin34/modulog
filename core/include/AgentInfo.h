#pragma once

#include <string>
#include <filesystem>
#include <utility>
#include "reproc++/reproc.hpp"

class AgentInfo {
public:
    AgentInfo(const std::string &agentId, const std::filesystem::path &agentPath, const std::shared_ptr<reproc::process>& agentProcess):
            agentId_(agentId), agentPath_(agentPath), agentProcess_(agentProcess){}
    void setAgentId(std::string id);
    std::string getAgentId();
    std::shared_ptr<reproc::process> getAgentProcess();
private:
    std::string agentId_;
    std::filesystem::path agentPath_;
    std::shared_ptr<reproc::process> agentProcess_;
};