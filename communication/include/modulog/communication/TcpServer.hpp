#pragma once

#include <modulog/communication/TcpConnection.hpp>
#include <modulog/meta_lib/SharedSettings.hpp>

#include <bringauto/logging/Logger.hpp>
#include <asio.hpp>

#include <iostream>

namespace modulog::communication{
    /**
     * This class represents a TCP server. It is used by the Core - agents connect to this server
     *  after creation and Core can communicate with them via TCP
     */
    class TcpServer {
    public:
        /**
         * @param io_context asio io context
         * @param messageMutex for syncing condition variable
         * @param messageConditionVariable connection will notify this variable if message arrives
         * @param totalReceivedMsgs counting received messages
         */
        TcpServer(asio::io_context &io_context, std::mutex &messageMutex, std::condition_variable &messageConditionVariable,
                  int &totalReceivedMsgs, std::shared_ptr<meta_lib::SharedSettings> sharedSettings)
                : io_context_(io_context),
                  acceptor_(io_context, asio::ip::tcp::endpoint(asio::ip::tcp::v4(), sharedSettings->ServerSettings.portNumber)),
                  messageMutex_(messageMutex), messageConditionVariable_(messageConditionVariable),
                  totalReceivedMsgs_(totalReceivedMsgs){}

        /**
         * Start accepting incoming connections
         */
        void startAccept();

        /**
         * Pop connection to work with it later
         * @return connection or nullptr if no connection was made
         */
        std::shared_ptr<TcpConnection> popConnection();

    private:
        /**
         * If someone connects, this function will handle it - push connectin to vector and accept other connections
         * @param new_connection made connection
         * @param error error flag
         */
        void handleAccept(std::shared_ptr<TcpConnection> new_connection, const asio::error_code &error);

        // ---- attributes
        asio::io_context &io_context_;
        asio::ip::tcp::acceptor acceptor_;
        std::vector<std::shared_ptr<TcpConnection>> lastConnectionsVector;
        std::string serverName_ = "server";
        // Synchro vars:
        std::mutex &messageMutex_;
        std::condition_variable &messageConditionVariable_;
        int &totalReceivedMsgs_;
    };
}
