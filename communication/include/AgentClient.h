#pragma once
#include <asio.hpp>
#include "TcpConnection.h"

class AgentClient {
public:
    AgentClient(std::shared_ptr<asio::io_context> &ioContext, bool isDebug, std::string agentName);
    std::string getAgentConfig();
    void sendLog(const std::shared_ptr<LogMessage>& logMessage);
    static std::string execCommand(const std::string& cmd);

private:
    void initClient();
    std::string agentName_ = "AgentDefaultName";
    std::string agentConfig_ = "default config... ";
    std::shared_ptr<asio::io_context> ioContext_;
    bool isDebug_;
    TcpConnection::pointer connection_;
    std::thread clientThread;
    void handleResponses();

    //TODO end clientThread
};

