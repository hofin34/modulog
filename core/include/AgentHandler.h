#pragma once

#include <vector>
#include "Agent.h"
#include "AgentProcess.h"
#include <fstream>

/**
 * Class responsible for managing agents
 */
class AgentHandler {
public:
    /**
     * @param pathToEnabledAgentsList path to file, where is specified, which agents will be created
     */
    AgentHandler(const std::filesystem::path& pathToEnabledAgentsList);
    /**
     * Run next agent process (it is getting agents from pathToEnabledAgentsList)
     * @return running process included in AgentInfo
     */
    std::shared_ptr<AgentProcess> runNextAgent();

    /**
     * Add to vector next agent to manage
     * @param messageExchanger already connected with agent and is ready to communicate with him (send/receive messages)
     * @param agentInfo containing already running process
     */
    void addNewAgent(const std::shared_ptr<MessageExchanger> &messageExchanger, const std::shared_ptr<AgentProcess> &agentInfo);

    const std::vector<std::shared_ptr<Agent>>& getRunningAgents();
    /**
     * Removes agent from vector of running agents and clean after agent (close connection and stop process)
     * @param agent Agent to be deleted
     */
    void deleteAgent(const std::shared_ptr<Agent>& agent);

private:
    std::vector<std::shared_ptr<Agent>> runningAgents_;
    std::vector<std::filesystem::path> agentsPaths_;
    void parseEnabledAgentsList(const std::filesystem::path& pathToListFile);
    int createdAgentsCount_ = 0;

};
