#pragma once

#include <memory>
#include <asio.hpp>
#include <modulog/communication/MessageExchanger.hpp>
#include <modulog/agent_client/AgentClient.hpp>

namespace modulog::agent_client {
    class TcpAgentClient : public AgentClient {
    public:
        /**
         * @param ioContext asio io context
         * @param agentName under which name will be saved logs later
         */
        TcpAgentClient(std::shared_ptr<asio::io_context> &ioContext, std::string agentName);


        ~TcpAgentClient();

        /**
         * Sends log to the Core, which will save it. If is client in debug mode, log is just printed
         * @param logMessage Message to log
         */
        void sendLog(const std::shared_ptr<communication::LogMessage> &logMessage) override;

        /**
         * Sends control message to the Core
         * @param controlMessage control message to send
         */
        void sendControl(const std::shared_ptr<communication::ControlMessage> &controlMessage) override;

        /**
         * Client connects to the Core, sends its name and starts
         * 2 new threads: first to collect incoming messages from Core and second in which io_context is running
         */
        void initClient() override;

        /**
         * Sends to Core exit message
         */
        void exitConnection() override;

        /**
         * Get shared config - it is sent to all agents on the beginning (what will be sent is defined
         * in SharedSettings file
         */
        std::string getSharedConfig() override;

        /**
         * You can log until this function returns true
         * @return true, if can be logs sent to the core.
         */
        bool canLog() override;

        /**
         * Sleep this thread for sleepTime sec - when is agent interrupted by Core, it stops waiting.
         * @param sleepTime how long wait (in seconds)
         * @return true if waited whole time, false if was interrupted during waiting
         */
        bool sleepFor(const std::chrono::seconds sleepTime) override;

        /**
         * reproc++ library sends interrupt signal to all spawned processes - we want to catch it and ignore, because
         * agent exiting is managed by Core.cxx
         */
        static void signalHandler(const std::error_code &error, int signum);

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


