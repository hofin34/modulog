#pragma once
#include <asio.hpp>
#include "ControlMessage.h"
#include "MessageSerializer.h"
#include <iostream>

class TcpConnection: public std::enable_shared_from_this<TcpConnection> {
public:
    typedef std::shared_ptr<TcpConnection> pointer;
    void start_read();
    void alive_handler();
    void send_alive();
    void start_alive_writer();
    asio::ip::tcp::socket& get_socket();
    static pointer create(asio::io_context& io_context);

private:
    TcpConnection(asio::io_context& io_context) : socket_(io_context), aliveTimer_(io_context), aliveResponseTimer_(io_context){}
    void handle_read(const asio::error_code& error, size_t bytes_transferred);
    // ------ Attributes
    bool waitingForACKAlive = false;
    asio::steady_timer aliveTimer_;
    asio::steady_timer aliveResponseTimer_;
    asio::ip::tcp::socket socket_;
    asio::streambuf input_buffer_;
};