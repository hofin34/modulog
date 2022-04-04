#pragma once

#include <modulog/watchdog_agent/WatchdogHandler.hpp>

#include <string>
#include <iostream>
#include <asio.hpp>

namespace modulog::watchdog_agent {
    /**
     * Class representing udp server - it is listening on some port and listening for messages from devices, that are broadcasting
     */
    class UdpServer {
    public:
        /**
         *
         * @param ioContext
         * @param port where will be server listening
         * @param watchdogHandler instance of watchdog handler - it is processing received messages
         */
        UdpServer(std::shared_ptr<asio::io_context> &ioContext, int port,
                  std::shared_ptr<WatchdogHandler> watchdogHandler);

    private:
        /**
         * Start receiving messages
         */
        void startReceive();

        /**
         * Called, when is message received - message then passed to watchdog handler
         * @param error
         */
        void handleReceive(const asio::error_code &error,
                           std::size_t /*bytes_transferred*/);

        asio::ip::udp::socket socket_;
        asio::ip::udp::endpoint remoteEndpoint_;
        std::array<char, 1024> recvBuffer_{'\0'};
        std::shared_ptr<WatchdogHandler> watchdogHandler_;
    };
}
