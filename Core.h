#pragma once
#include <asio.hpp>
#include "AgentHandler.h"

class Core {
public:
    Core(const std::filesystem::path& pathToAgentsConfigs);
    void start();
    void startSendAlive();

private:
    AgentHandler agentHandler_;
};

