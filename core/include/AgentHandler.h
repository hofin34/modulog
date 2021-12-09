#pragma once

#include <vector>
#include "Agent.h"
#include "AgentInfo.h"
#include <fstream>

class AgentHandler {
public:
    AgentHandler(const std::filesystem::path& pathToAgentsConfigDir);
    std::shared_ptr<AgentInfo> runNextAgent();
    void addNewAgent(std::shared_ptr<MessageExchanger> messageExchanger, std::shared_ptr<AgentInfo> agentInfo);
    const std::vector<std::shared_ptr<Agent>>& getRunningAgents();
    void deleteAgent(const std::shared_ptr<Agent>& agent);
    void deleteAgent(const std::string &agentId);

private:
    std::vector<std::shared_ptr<Agent>> runningAgents_;
    std::vector<std::filesystem::path> agentsPaths_;
    static void cleanup(int sigNum);
    void loadAgentsConfigs(const std::filesystem::path& pathToListFile);
    int createdAgentsCount_ = 0;

};
