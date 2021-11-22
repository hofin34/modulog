#pragma once

#include <vector>
#include "Agent.h"

class AgentHandler {
public:
    std::shared_ptr<Agent> createNextAgent();

private:
    std::vector<std::shared_ptr<Agent>> running_agents;
    static void cleanup(int sigNum);
    int createAgents();
};
