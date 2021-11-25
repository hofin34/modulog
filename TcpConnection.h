#pragma once
#include <asio.hpp>
#include "ControlMessage.h"
#include "MessageSerializer.h"
#include <iostream>

class TcpConnection: public std::enable_shared_from_this<TcpConnection> {
public:
    const int MAX_PACKET_SIZE = 512;
    typedef std::shared_ptr<TcpConnection> pointer;
    void send_message(std::string& msg);
    void start_read();
    asio::ip::tcp::socket& get_socket();
    static pointer create(asio::io_context& io_context);
    std::shared_ptr<std::string> popMessage();
    bool isMessage();

private:
    TcpConnection(asio::io_context& io_context) : socket_(io_context){
        messagesVector_ = std::make_shared<std::vector<std::string>>();
        msgBuffer_ = std::make_shared<asio::streambuf>(128);
    } //TODO specify buff size
    void handle_read_msg_size(const asio::error_code& error, size_t bytes_transferred);
    void read_msg_content();
    void handle_read_msg_content(const asio::error_code& error, size_t bytes_transferred);
    // ------ Attributes
    asio::ip::tcp::socket socket_;
    uint32_t msgLength = 0;
    std::shared_ptr<asio::streambuf> msgBuffer_;
    std::shared_ptr<std::vector<std::string>> messagesVector_;
    int alreadyRead_ = 0;
    std::string finalMessage_;
};