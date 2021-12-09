#pragma once
#include <iostream>
#include <asio.hpp>
#include "TcpConnection.h"
#include "../include/TcpConnection.h"
#include "../include/MessageDeserializer.h"
#include "MessageExchanger.h"
#include <nlohmann/json.hpp>

class AgentClient {
public:
    AgentClient(std::shared_ptr<asio::io_context> &ioContext, bool isDebug, std::string agentName);
    std::string getAgentConfig();
    void sendLog(const std::shared_ptr<LogMessage>& logMessage);
    void sendControl(const std::shared_ptr<ControlMessage> &controlMessage);
    void initClient();
    void exitConnection();

    //TODO move to helper funcs:
    static std::string execCommand(const std::string& cmd);
    static nlohmann::json parseConfig(const std::string& execPath);


private:
    std::string agentName_ = "AgentDefaultName";
    std::string agentConfig_ = "default config... ";
    std::shared_ptr<asio::io_context> ioContext_;
    bool isDebug_;
    std::thread clientThread;
    std::thread responseHandleThread;
    void handleResponses();
    std::shared_ptr<MessageExchanger> messageExchanger_;

    // Sync vars:
    std::atomic<bool> exitConfirmed = false;
    std::mutex msgMutex_;
    std::condition_variable msgCondVar_;
    int totalMsgsReceived_ = 0;
    std::shared_ptr<MessageProcessor> msgProcessor_;

    //TODO end clientThread
};

