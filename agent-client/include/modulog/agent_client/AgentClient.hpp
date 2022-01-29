#pragma once
#include <modulog/communication/TcpConnection.hpp>
#include <modulog/communication/MessageDeserializer.hpp>
#include <modulog/communication/MessageExchanger.hpp>
#include <modulog/communication/LogMessage.hpp>
#include <modulog/communication/ControlMessage.hpp>

#include <nlohmann/json.hpp>

#include <iostream>
#include <filesystem>

namespace modulog::agent_client{
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
        void sendLog(const std::shared_ptr<communication::LogMessage>& logMessage);
        /**
         * Sends control message to the Core
         * @param controlMessage control message to send
         */
        void sendControl(const std::shared_ptr<communication::ControlMessage> &controlMessage);
        /**
         * Client connects to the Core, sends its name and starts
         * 2 new threads: first to collect incoming messages from Core and second in which io_context is running
         */
        void initClient();

        /*
         * Sends to Core exit message
         */
        void exitConnection();

        std::string getSharedConfig();


    private:
        std::string agentName_ = "AgentDefaultName";
        std::shared_ptr<asio::io_context> ioContext_;
        bool isDebug_;
        std::thread clientThread;
        std::thread responseHandleThread;
        void handleResponses();
        std::shared_ptr<communication::MessageExchanger> messageExchanger_;
        std::string sharedConfig;

        // Sync vars:
        std::atomic<bool> confirmedExit_;
        std::mutex msgMutex_;
        std::condition_variable msgCondVar_;
        int totalMsgsReceived_ = 0;
        std::shared_ptr<communication::MessageProcessor> msgProcessor_;

        //TODO end clientThread
    };

}
