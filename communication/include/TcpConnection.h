#pragma once
#include <asio.hpp>
#include "ControlMessage.h"
#include "MessageSerializer.h"
#include "MessageProcessor.h"
#include <iostream>
#include <utility>
//#include <AgentHandler.h>
//#include "../../core/include/AgentHandler.h"
class TcpConnection: public std::enable_shared_from_this<TcpConnection> {
public:
    const int MAX_PACKET_SIZE = 512;
    typedef std::shared_ptr<TcpConnection> pointer;
    void send_message(const std::string& msg);
    void start_read();
    asio::ip::tcp::socket& get_socket();
    static pointer create(asio::io_context& io_context, std::string& connectionName, std::shared_ptr<MessageProcessor> messageProcessor);
    std::shared_ptr<MessageProcessor> getMessageProcessor_();

private:
    TcpConnection(asio::io_context& io_context, std::string& connectionName, std::shared_ptr<MessageProcessor> messageProcessor) : socket_(io_context), connectionName_(connectionName),
        messageProcessor_(std::move(messageProcessor)){
        msgBuffer_ = std::make_shared<asio::streambuf>(128);
       // messageProcessor_->setConnection(this);
    } //TODO specify buff size
    void handle_read_msg_size(const asio::error_code& error, size_t bytes_transferred);
    void read_msg_content();
    void handle_read_msg_content(const asio::error_code& error, size_t bytes_transferred);
    void signal_err_exit();


    // ------ Attributes
    asio::ip::tcp::socket socket_;
    uint32_t msgLength = 0;
    std::shared_ptr<asio::streambuf> msgBuffer_;
    int alreadyRead_ = 0;
    std::string finalMessage_;
    std::string connectionName_;
    std::shared_ptr<MessageProcessor>messageProcessor_;
    // To cleanup after connection end:
};