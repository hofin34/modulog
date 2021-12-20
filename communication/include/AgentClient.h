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
    /**
     * @param ioContext asio io context
     * @param isDebug if true, you can use client standalone - client
     *  is not connecting to server and if you send log, it is just printed but not sent
     * @param agentName under which name will be saved logs later
     */
    AgentClient(std::shared_ptr<asio::io_context> &ioContext, bool isDebug, std::string agentName);
    /**
     * Sends log to the Core, which will save it. If is client in debug mode, log is just printed
     * @param logMessage Message to log
     */
    void sendLog(const std::shared_ptr<LogMessage>& logMessage);
    /**
     * Sends control message to the Core
     * @param controlMessage control message to send
     */
    void sendControl(const std::shared_ptr<ControlMessage> &controlMessage);
    /**
     * Client connects to the Core, sends its name and starts
     * 2 new threads: first to collect incoming messages from Core and second in which io_context is running
     */
    void initClient();

    /*
     * Sends to Core exit message
     */
    void exitConnection();




private:
    std::string agentName_ = "AgentDefaultName";
    std::shared_ptr<asio::io_context> ioContext_;
    bool isDebug_;
    std::thread clientThread;
    std::thread responseHandleThread;
    void handleResponses();
    std::shared_ptr<MessageExchanger> messageExchanger_;

    // Sync vars:
    std::mutex msgMutex_;
    std::condition_variable msgCondVar_;
    int totalMsgsReceived_ = 0;
    std::shared_ptr<MessageProcessor> msgProcessor_;

    //TODO end clientThread
};

