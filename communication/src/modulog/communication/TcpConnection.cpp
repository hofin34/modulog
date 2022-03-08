#include <modulog/communication/TcpConnection.hpp>

namespace modulog::communication {

    asio::ip::tcp::socket &TcpConnection::getSocket() {
        return socket_;
    }

    void TcpConnection::startRead() {
        asio::async_read(socket_, asio::buffer(&msgLength, sizeof(msgLength)),
                         asio::transfer_exactly(sizeof(msgLength)),
                         [this](const asio::error_code &ec, size_t bytes_transfered) {
                             TcpConnection::handleReadMsgSize(ec, bytes_transfered);
                         });
    }


    void TcpConnection::handleReadMsgSize(const asio::error_code &error,
                                          size_t bytes_transferred) {
        if (!error) {
            msgBuffer_ = std::make_shared<asio::streambuf>(msgLength);
            readMsgContent();
        } else {
            if(!connectionClosed_.load())
                signalErrExit();
            return;
        }
    }

    void TcpConnection::readMsgContent() {
        int restToRead = msgLength - alreadyRead_;
        int toTransfer = 0;
        if (restToRead > MAX_PACKET_SIZE) {
            toTransfer = MAX_PACKET_SIZE;
        } else {
            toTransfer = restToRead;
        }
        asio::async_read(socket_, *msgBuffer_, asio::transfer_exactly(toTransfer),
                         [this](const asio::error_code &error, size_t bytes_transfered) {
                             handleReadMsgContent(error, bytes_transfered);
                         });

    }

    void TcpConnection::handleReadMsgContent(const asio::error_code &error, size_t bytes_transferred) {
        if (!error) {
            alreadyRead_ += bytes_transferred;
            std::istream istream(&(*msgBuffer_));
            std::string msgPart(std::istreambuf_iterator<char>(istream), {});
            finalMessage_ += msgPart;
            msgBuffer_->consume(bytes_transferred);
            if (alreadyRead_ != msgLength) {
                readMsgContent();
            } else {
                alreadyRead_ = 0;
                messageProcessor_->processMessage(finalMessage_);
                finalMessage_ = "";
                startRead();
            }
        } else {
            bringauto::logging::Logger::logError("{} :error in reading msg content: {}", connectionName_, error.message());
            if(!connectionClosed_.load())
                signalErrExit();
            return;
        }
    }

    void TcpConnection::sendMessage(const std::string &msg) {
        asio::error_code errorWrite;
        uint32_t msgSize = msg.length();
        std::array<asio::const_buffer,2> buffToSend = {
                asio::buffer(&msgSize, sizeof(msgSize)),
                asio::buffer(msg)
        };
        asio::write(socket_, buffToSend, errorWrite);
        if (errorWrite) {
            if(!connectionClosed_.load())
                signalErrExit();
            bringauto::logging::Logger::logError("{}: error in sending: {}", connectionName_, errorWrite.message());
        }
    }


    std::shared_ptr<MessageProcessor> TcpConnection::getMessageProcessor() {
        return messageProcessor_;
    }

    void TcpConnection::signalErrExit() {
        bringauto::logging::Logger::logDebug("Signal err");
        auto exitControlMsg = std::make_shared<ControlMessage>(ControlMessage::CONTROL_MSG_TYPE::EXIT_ERR, "");
        MessageSerializer messageSerializer(exitControlMsg);
        messageProcessor_->processMessage(messageSerializer.serialize());
    }

    void TcpConnection::closeConnection() {
        bringauto::logging::Logger::logDebug("Closing connection");
        asio::error_code ec;
        socket_.shutdown(asio::ip::tcp::socket::shutdown_both, ec);
        if (ec)
            bringauto::logging::Logger::logError("Closing connection error: {}", ec.message());
        else{
            connectionClosed_ = true;
            socket_.close();
        }
    }
}
