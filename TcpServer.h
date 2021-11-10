#pragma once
#include <asio.hpp>
#include "TcpConnection.h"

class TcpServer {
public:
    TcpServer(asio::io_context& io_context)
    : io_context_(io_context),
            acceptor_(io_context, asio::ip::tcp::endpoint(asio::ip::tcp::v4(), 13)){}
    void start_accept();


private:
    void handle_accept(TcpConnection::pointer new_connection, const asio::error_code& error);
    // ---- attributes
    asio::io_context& io_context_;
    asio::ip::tcp::acceptor acceptor_;
};