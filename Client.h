#pragma once
#include <asio.hpp>
#include <iostream>
#include <thread>
class Client : public std::enable_shared_from_this<Client> {
public:

    Client(asio::io_context& io_context);


    void run();
    void start_write(std::string& msg);
    void log();
    void send_log(std::string& msg);
private:
    void handle_write(const asio::error_code& error,
                              size_t bytes_transferred);
    void start_read();
    void handle_read_content(const asio::error_code& error,
                     size_t bytes_transferred);
    void handle_read_msg_size(const asio::error_code &error, size_t bytes_transferred);
    asio::streambuf input_buffer_;
    uint32_t msgLength;
    asio::io_context &ioContext_;
    asio::ip::tcp::socket socket_;
    std::string msg_to_send_;
};