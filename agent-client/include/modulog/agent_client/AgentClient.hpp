#pragma once

#include <modulog/communication/TcpConnection.hpp>
#include <modulog/communication/MessageDeserializer.hpp>
#include <modulog/communication/MessageExchanger.hpp>
#include <modulog/communication/LogMessage.hpp>
#include <modulog/communication/ControlMessage.hpp>

#include <nlohmann/json.hpp>
#include <bringauto/logging/Logger.hpp>
#include <bringauto/logging/ConsoleSink.hpp>

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

        ~AgentClient();

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

        /**
         * Sends to Core exit message
         */
        void exitConnection();

        /**
         * Get shared config - it is sent to all agents on the beginning (what will be sent is defined
         * in SharedSettings file
         */
        std::string getSharedConfig();

        /**
         * You can log until this function returns true
         * @return true, if can be logs sent to the core.
         */
        bool canLog();

        /**
         * reproc++ library sends interrupt signal to all spawned processes - we want to catch it and ignore, because
         * agent exiting is managed by Core.cxx
         */
        static void signalHandler(int signum);

        /**
         * Sleep this thread for sleepTime sec - when is agent interrupted by Core, it stops waiting.
         * @param sleepTime how long wait (in seconds)
         * @return true if waited whole time, false if was interrupted during waiting
         */
        bool sleepFor(const std::chrono::seconds sleepTime);

    private:
        /**
         * Function running in own thread - it is receiving messages and processing them.
         * It is virtual, because it is overriden in tests to simulate freezed agent.
         */
        virtual void handleResponses();


        std::string agentName_ = "AgentDefaultName";
        std::shared_ptr<asio::io_context> ioContext_;
        std::thread clientThread_;
        std::thread responseHandleThread_;
        std::shared_ptr<communication::MessageExchanger> messageExchanger_;
        std::string sharedConfig_;
        std::string coreIp_ = "127.0.0.1";
        int corePort_ = 1234;
        // Sync vars waiting:
        std::condition_variable waitCondVar_;
        std::mutex waitMutex_;
        bool waitEnd_ = false;
        // Sync vars messages:
        std::atomic<bool> shouldExit_ = false;
        std::mutex msgMutex_;
        std::condition_variable msgCondVar_;
        int totalMsgsReceived_ = 0;
        std::shared_ptr<communication::MessageProcessor> msgProcessor_;
    };

}
