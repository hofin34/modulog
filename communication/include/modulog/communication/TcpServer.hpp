#pragma once

#include <asio.hpp>
#include "TcpConnection.hpp"

namespace modulog::communication{
    class TcpServer {
    public:
        /**
         * @param io_context asio io context
         * @param messageMutex for syncing condition variable
         * @param messageConditionVariable connection will notify this variable if message arrives
         * @param totalReceivedMsgs counting received messages
         */
        TcpServer(asio::io_context &io_context, std::mutex &messageMutex, std::condition_variable &messageConditionVariable,
                  int &totalReceivedMsgs)
                : io_context_(io_context),
                  acceptor_(io_context, asio::ip::tcp::endpoint(asio::ip::tcp::v4(), 1234)),
                  messageMutex_(messageMutex), messageConditionVariable_(messageConditionVariable),
                  totalReceivedMsgs_(totalReceivedMsgs) {}

        /**
         * Start accepting incoming connections
         */
        void startAccept();

        /**
         * Pop connection to work with it later
         * @return connection or nullptr if no connection was made
         */
        TcpConnection::pointer popConnection();

    private:
        /**
         * If someone connects, this function will handle it - push connectin to vector and accept other connections
         * @param new_connection made connection
         * @param error error flag
         */
        void handleAccept(TcpConnection::pointer new_connection, const asio::error_code &error);

        // ---- attributes
        asio::io_context &io_context_;
        asio::ip::tcp::acceptor acceptor_;
        std::vector<TcpConnection::pointer> lastConnectionsVector;
        std::string serverName_ = "server";
        // Synchro vars:
        std::mutex &messageMutex_;
        std::condition_variable &messageConditionVariable_;
        int &totalReceivedMsgs_;
    };
}
