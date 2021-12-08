#include "../include/TcpConnection.h"


TcpConnection::pointer TcpConnection::create(asio::io_context& io_context, std::string& connectionName, std::shared_ptr<MessageProcessor> messageProcessor)
{
    return pointer(new TcpConnection(io_context, connectionName, messageProcessor)); //TODO refactor creation...
}

asio::ip::tcp::socket& TcpConnection::get_socket()
{
    return socket_;
}

void TcpConnection::start_read()
{
    asio::async_read(socket_, asio::buffer(&msgLength, sizeof(msgLength)), asio::transfer_exactly(sizeof(msgLength)),
                           std::bind(&TcpConnection::handle_read_msg_size, shared_from_this(), std::placeholders::_1, std::placeholders::_2));
}

void TcpConnection::handle_read_msg_size(const asio::error_code& error,
                                         size_t bytes_transferred)
{
    if(error == asio::error::eof){
        std::cerr << connectionName_ << " err: " << error.message() << " (maybe connection closed?)";
        throw std::runtime_error(error.message());

    }
    if(!error){
        std::cout << "Msg size: " << msgLength << std::endl;
        msgBuffer_ = std::make_shared<asio::streambuf>(msgLength);
        read_msg_content();
    }else{
        std::cerr << connectionName_ + " err in reading msg size: " + error.message();
        throw std::runtime_error(connectionName_ + " err in reading msg size: " + error.message());
    }
 }

 void TcpConnection::read_msg_content(){
    int restToRead = msgLength - alreadyRead_;
    if(restToRead >MAX_PACKET_SIZE){
        asio::async_read(socket_, *msgBuffer_,asio::transfer_exactly(MAX_PACKET_SIZE),
                         std::bind(&TcpConnection::handle_read_msg_content, shared_from_this(), std::placeholders::_1, std::placeholders::_2));
    }else{
        asio::async_read(socket_, *msgBuffer_,asio::transfer_exactly(restToRead),
                         std::bind(&TcpConnection::handle_read_msg_content, shared_from_this(), std::placeholders::_1, std::placeholders::_2));
    }

}

void TcpConnection::handle_read_msg_content(const asio::error_code &error, size_t bytes_transferred) {
    if(!error){
        alreadyRead_ += bytes_transferred;
        std::cout << "Content bytes read: " << alreadyRead_ << "/" << msgLength <<std::endl;
        std::istream istream(&(*msgBuffer_));
        std::string msgPart(std::istreambuf_iterator<char>(istream), {});
        finalMessage_ += msgPart;
        msgBuffer_->consume(bytes_transferred);
        if(alreadyRead_ != msgLength){
            read_msg_content();
        }else{
            alreadyRead_ = 0;
            messagesVector_->push_back(finalMessage_);
            std::cout << connectionName_ << " received: " << finalMessage_ << std::endl;
            messageProcessor_->processMessage(finalMessage_);
            finalMessage_ = "";
            start_read();
        }
    }else{
        throw std::runtime_error(connectionName_ + ": error in reading msg content: " + error.message());
    }
}

void TcpConnection::send_message(const std::string& msg) {
    asio::error_code errorWrite;
    uint32_t msgSize = msg.length();
    asio::write(socket_, asio::buffer(&msgSize, sizeof(msgSize)), errorWrite); // TODO these writes on one line?
    asio::write(socket_, asio::buffer(msg), errorWrite);
    if(errorWrite){
        std::cerr << errorWrite.message();
        throw std::runtime_error("Error in sending: " + errorWrite.message());
    }
}



std::shared_ptr<MessageProcessor> TcpConnection::getMessageProcessor_() {
    return messageProcessor_;
}


