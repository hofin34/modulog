#pragma once

#include <vector>
#include "Agent.h"

class AgentHandler {
public:
    AgentHandler(): runningAgents_(){};
    std::shared_ptr<Agent> createNextAgent();
    const std::vector<std::shared_ptr<Agent>>& getRunningAgents();


private:
    std::vector<std::shared_ptr<Agent>> runningAgents_;
    static void cleanup(int sigNum);
    int createAgents();
};
