#pragma once
#include <asio.hpp>
#include <iostream>
#include <thread>
class Client : public std::enable_shared_from_this<Client> {
public:

    Client(asio::io_context& io_context);


    void run();
    void send_msg(std::string& msg);
    std::string getFrontMessage();
    std::string popMessage();
    std::vector<std::string> getMessagesVector();

        private:
    void start_read();
    void handle_read_msg_size(const asio::error_code &error, size_t bytes_transferred);
    void read_msg_content();
    void handle_read_msg_content(const asio::error_code &error, size_t bytes_transferred);


        asio::streambuf input_buffer_;
    uint32_t msgLength;
    asio::io_context &ioContext_;
    asio::ip::tcp::socket socket_;
    std::string msg_to_send_;

    asio::streambuf msgBuffer_;
    std::vector<std::string> messagesVector_;
    int alreadyRead_ = 0;
    std::string finalMessage_;
};