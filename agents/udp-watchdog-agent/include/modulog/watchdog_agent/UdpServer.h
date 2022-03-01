#pragma once

#include <modulog/watchdog_agent/WatchdogHandler.h>

#include <string>
#include <iostream>
#include <asio.hpp>

namespace modulog::watchdog_agent{
    class UdpServer {
    public:
        UdpServer(asio::io_context &ioContext, int port, std::shared_ptr<WatchdogHandler> watchdogHandler);

    private:
        void startReceive();

        void handleReceive(const asio::error_code &error,
                           std::size_t /*bytes_transferred*/);

        asio::ip::udp::socket socket_;
        asio::ip::udp::endpoint remoteEndpoint_;
        std::array<char, 1024> recvBuffer_{'\0'};
        std::shared_ptr<WatchdogHandler> watchdogHandler_;
    };
}
