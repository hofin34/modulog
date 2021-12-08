#pragma once
#include <asio.hpp>
#include "AgentHandler.h"
#include "TcpServer.h"

class Core {
public:
    Core(const std::filesystem::path& pathToAgentsConfigs, std::shared_ptr<asio::io_context> ioContext);
    void start();
    void startSendAlive();

private:
    std::shared_ptr<AgentHandler> agentHandler_;
    asio::steady_timer sendAliveTimer_;
    std::shared_ptr<asio::io_context> ioContext_;
    void sendAlive();
    void checkIfAgentsAlive();
    void initAllAgents();
    void notifyAllAgentsToSendLogs();
    TcpServer server_;
    //sync vars:
    int totalReceivedMessages_ = 0;
    std::mutex messageMutex_;
    std::condition_variable messageConditionVar_;
};

