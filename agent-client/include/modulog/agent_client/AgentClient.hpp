#pragma once

#include <modulog/communication/TcpConnection.hpp>
#include <modulog/communication/MessageDeserializer.hpp>
#include <modulog/communication/MessageExchanger.hpp>
#include <modulog/communication/LogMessage.hpp>
#include <modulog/communication/ControlMessage.hpp>

#include <nlohmann/json.hpp>
#include <bringauto/logging/Logger.hpp>

#include <iostream>
#include <filesystem>

namespace modulog::agent_client {
    /**
     * This class is used by agents to easily send logs to the core.
     * At the beginning has to be initialized via initClient() and can be sent logs
     * with function sendLog(). In the background, class responds to IS_ALIVE messages
     * and process other messages. You can enable debug mode by setting AGENT_CLIENT_DEBUG macro.
     * If set, client doesn't try to connect to Core and just prints messages, that should be sent normally.
     */
    class AgentClient {
    public:
        /**
         * @param ioContext asio io context
         * @param agentName under which name will be saved logs later
         */
        AgentClient(std::shared_ptr<asio::io_context> &ioContext, std::string agentName);

        /**
         * Sends log to the Core, which will save it. If is client in debug mode, log is just printed
         * @param logMessage Message to log
         */
        void sendLog(const std::shared_ptr<communication::LogMessage> &logMessage);

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

        /*
         * Get shared config - it is sent to all agents on the beginning (what will be sent is defined
         * in SharedSettings file
         */
        std::string getSharedConfig();

        /*
         * reproc++ library sends interrupt signal to all spawned processes - we want to catch it and ignore, because
         * agent exiting is managed by Core.cxx
         */
        static void signalHandler(int signum);

        /*
         * For testing purposes - client stops responding to IS_ALIVE messages
         */
        void freezeClient();

    private:
        /*
         * Function running in own thread - it is receiving messages and processing them
         */
        void handleResponses();

        std::string agentName_ = "AgentDefaultName";
        std::shared_ptr<asio::io_context> ioContext_;
        std::thread clientThread_;
        std::thread responseHandleThread_;
        std::shared_ptr<communication::MessageExchanger> messageExchanger_;
        std::string sharedConfig_;
        std::atomic<bool> simulatedFreeze = false;
        std::string coreIp_ = "127.0.0.1";
        int corePort_ = 1234;
        // Sync vars:
        std::atomic<bool> shouldExit_;
        std::mutex msgMutex_;
        std::condition_variable msgCondVar_;
        int totalMsgsReceived_ = 0;
        std::shared_ptr<communication::MessageProcessor> msgProcessor_;
    };

}
