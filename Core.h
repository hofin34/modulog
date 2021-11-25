#pragma once
#include <asio.hpp>
#include "AgentHandler.h"

class Core {
public:
    Core(const std::filesystem::path& pathToAgentsConfigs, std::shared_ptr<asio::io_context> ioContext);
    void start();
    void startSendAlive();

private:
    AgentHandler agentHandler_;
    asio::steady_timer sendAliveTimer_;
    std::shared_ptr<asio::io_context> ioContext_;
    void sendAlive();
    void checkIfAgentsAlive();
};

