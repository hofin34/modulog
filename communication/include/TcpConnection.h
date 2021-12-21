#pragma once

#include <asio.hpp>
#include "ControlMessage.h"
#include "MessageSerializer.h"
#include "MessageProcessor.h"
#include <iostream>
#include <utility>

class TcpConnection : public std::enable_shared_from_this<TcpConnection> {
public:
    const int MAX_PACKET_SIZE = 512; // how big packets are read
    typedef std::shared_ptr<TcpConnection> pointer;

    /**
     * Sends message msg
     * @param msg message to be sent
     */
    void sendMessage(const std::string &msg);

    /**
     * Start reading messages - should be called after connection creation
     * First reads message size and then content
     */
    void startRead();

    /**
     * Get connection socket
     * @return socket
     */
    asio::ip::tcp::socket &getSocket();

    static pointer create(asio::io_context &io_context, std::string &connectionName,
                          std::shared_ptr<MessageProcessor> messageProcessor);

    /**
     * shutdowns socket
     */
    void closeConnection();

    /**
     * Gets MessageProcessor - used for processing messages
     * @return MessageProcessor
     */
    std::shared_ptr<MessageProcessor> getMessageProcessor();

private:
    TcpConnection(asio::io_context &io_context, std::string &connectionName,
                  std::shared_ptr<MessageProcessor> messageProcessor) : socket_(io_context),
                                                                        connectionName_(connectionName),
                                                                        messageProcessor_(std::move(messageProcessor)) {
        msgBuffer_ = std::make_shared<asio::streambuf>();
    }

    void handleReadMsgSize(const asio::error_code &error, size_t bytes_transferred);

    void readMsgContent();

    void handleReadMsgContent(const asio::error_code &error, size_t bytes_transferred);

    void signalErrExit();


    // ------ Attributes
    asio::ip::tcp::socket socket_;
    uint32_t msgLength = 0;
    std::shared_ptr<asio::streambuf> msgBuffer_;
    int alreadyRead_ = 0;
    std::string finalMessage_;
    std::string connectionName_;
    std::shared_ptr<MessageProcessor> messageProcessor_;
};