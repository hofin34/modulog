#pragma once

#include <modulog/communication/LogMessage.hpp>
#include <modulog/communication/ControlMessage.hpp>

#include <nlohmann/json.hpp>
#include <bringauto/logging/Logger.hpp>
#include <bringauto/logging/ConsoleSink.hpp>

#include <iostream>
#include <filesystem>
#include <thread>

namespace modulog::agent_client {
    /**
     * This class is used by agents to easily send logs to the core.
     * At the beginning has to be initialized via initClient() and can be sent logs
     * with function sendLog(). In the background, class responds to IS_ALIVE messages
     * and process other messages. You can enable debug mode by setting AGENT_CLIENT_STANDALONE macro.
     * If set, client doesn't try to connect to Core and just prints messages, that should be sent normally.
     */
    class AgentClient {
    public:
        /**
         * Sends log to the Core, which will save it. If is client in debug mode, log is just printed
         * @param logMessage Message to log
         */
        virtual void sendLog(const std::shared_ptr<communication::LogMessage> &logMessage) = 0;

        /**
         * Sends control message to the Core
         * @param controlMessage control message to send
         */
        virtual void sendControl(const std::shared_ptr<communication::ControlMessage> &controlMessage) = 0;

        /**
         * Client connects to the Core, sends its name and starts
         * 2 new threads: first to collect incoming messages from Core and second in which io_context is running
         */
        virtual void initClient() = 0;

        /**
         * Sends to Core exit message
         */
        virtual void exitConnection() = 0;

        /**
         * Get shared config - it is sent to all agents on the beginning (what will be sent is defined
         * in SharedSettings file
         */
        virtual std::string getSharedConfig() = 0;

        /**
         * You can log until this function returns true
         * @return true, if can be logs sent to the core.
         */
        virtual bool canLog() = 0;


        /**
         * Sleep this thread for sleepTime sec - when is agent interrupted by Core, it stops waiting.
         * @param sleepTime how long wait (in seconds)
         * @return true if waited whole time, false if was interrupted during waiting
         */
        virtual bool sleepFor(const std::chrono::seconds sleepTime) = 0;

    protected:
        void generalInit(std::string agentName);

    };

}
