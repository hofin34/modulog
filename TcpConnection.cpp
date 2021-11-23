#include "TcpConnection.h"


TcpConnection::pointer TcpConnection::create(asio::io_context& io_context)
{
    return pointer(new TcpConnection(io_context)); //TODO refactor creation...
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
        start_read();
    }
    if(!error){
        std::cout << "Msg size: " << msgLength << std::endl;
        msgBuffer_ = std::make_shared<asio::streambuf>(msgLength);
        read_msg_content();
    }else{
        throw std::runtime_error("Server err in reading msg size: " + error.message());
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
            std::cout << "Server received: " << finalMessage_ << std::endl;
            finalMessage_ = "";
            start_read();
        }
    }else{
        throw std::runtime_error("Error in reading msg content: " + error.message());
    }
}

void TcpConnection::send_message(std::string& msg) { //TODO not tested
    asio::error_code errorWrite;
    uint32_t msgSize = msg.length();
    asio::write(socket_, asio::buffer(&msgSize, sizeof(msgSize)), errorWrite);
    asio::write(socket_, asio::buffer(msg), errorWrite);
    if(errorWrite){
        std::cerr << errorWrite.message();
        throw std::runtime_error("Error in sending: " + errorWrite.message());
    }
}

std::shared_ptr<std::vector<std::string>> TcpConnection::getMessagesVector() {
    return messagesVector_;
}

std::string TcpConnection::getFrontMessage() {
    return messagesVector_->front();
}

std::string TcpConnection::popMessage() {
    auto msgToReturn = getFrontMessage();
    messagesVector_->erase(messagesVector_->begin());
    return msgToReturn;
}
