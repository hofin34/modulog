#pragma once

#include <vector>
#include "Agent.h"

class AgentHandler {
    std::vector<Agent> running_agents;
    static void cleanup(int sigNum);
    int createAgents();
};
