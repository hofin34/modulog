#pragma once
#include <asio.hpp>
#include "AgentHandler.h"

class Core {
public:
    void start();

private:
    AgentHandler agentHandler;
};

