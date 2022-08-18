#pragma once

#include <modulog/communication/TcpServer.hpp>
#include <modulog/communication/MessageDeserializer.hpp>
#include <modulog/meta_lib/SharedSettings.hpp>
#include <modulog/core/LogSaver.hpp>
#include <modulog/core/AgentHandler.hpp>

#include <thread>

namespace modulog::core {
    /**
     * Core class - takes care of creating agents, connecting to them, checking if they are alive and collecting their logs
     */
    class Core {
    public:
        Core(const std::shared_ptr<asio::io_context>& ioContext,
             const std::shared_ptr<meta_lib::SharedSettings>& sharedSettings);

        ~Core();

        /**
         * Start core functionality after creating instance of this class
         */
        void start();

        /**
         * Function used to stop receiving messages in the Core
         */
        void stop();

    private:
        /**
         * Start sending isAlive - just starting async timer and then calls handler
         */
        void startSendAlive();

        /**
         * Sends isAlive message to all agents
         */
        void sendAlive();

        /**
         * Check all agents, if they responded to isAlive. If not, they are killed and removed
         */
        void checkIfAgentsAlive();

        /**
         * Run all agents and init connection with them
         */
        void initAllAgents();

        /**
         * Notify all running agents, that they can start sending logs
         */
        void notifyAllAgentsToSendLogs();

        void cleanAll();

        std::atomic<bool> stopFlag = false;
        std::shared_ptr<AgentHandler> agentHandler_;
        asio::steady_timer sendAliveTimer_;
        std::shared_ptr<asio::io_context> ioContext_;
        communication::TcpServer server_;
        std::thread serverThread_;
        std::shared_ptr<meta_lib::SharedSettings> sharedSettings_;
        //sync vars:
        int totalReceivedMessages_ = 0;
        std::mutex messageMutex_;
        std::condition_variable messageConditionVar_;
    };


}
