#include "Client.h"
#include "MessageDeserializer.h"
#include "MessageSerializer.h"

Client::Client(asio::io_context& io_context) : socket_(io_context), ioContext_(io_context){
    msgBuffer_ = std::make_shared<asio::streambuf>(128);
    messagesVector_ = std::make_shared<std::vector<std::string>>();

    try{
        asio::ip::tcp::endpoint endpoint(asio::ip::address::from_string("127.0.0.1"), 1234);
        socket_.connect(endpoint);
        //start_read();
        //std::string msgToSend = "Hello Lorem ipsum\nsome random\ntext.Hello Lorem ipsum\nsome random\ntext.Hello Lorem ipsum\nsome random\ntext.Hello Lorem ipsum\nsome random\ntext.Hello Lorem ipsum\nsome random\ntext.Hello Lorem ipsum\nsome random\ntext.Hello Lorem ipsum\nsome random\ntext.Hello Lorem ipsum\nsome random\ntext.Hello Lorem ipsum\nsome random\ntext.Hello Lorem ipsum\nsome random\ntext.Hello Lorem ipsum\nsome random\ntext.Hello Lorem ipsum\nsome random\ntext.Hello Lorem ipsum\nsome random\ntext.Hello Lorem ipsum\nsome random\ntext.Hello Lorem ipsum\nsome random\ntext.Hello Lorem ipsum\nsome random\ntext.Hello Lorem ipsum\nsome random\ntext.Hello Lorem ipsum\nsome random\ntext.Hello Lorem ipsum\nsome random\ntext.Hello Lorem ipsum\nsome random\ntext.Hello Lorem ipsum\nsome random\ntext.Hello Lorem ipsum\nsome random\ntext.Hello Lorem ipsum\nsome random\ntext.Hello Lorem ipsum\nsome random\ntext.Hello Lorem ipsum\nsome random\ntext.Hello Lorem ipsum\nsome random\ntext.Hello Lorem ipsum\nsome random\ntext.Hello Lorem ipsum\nsome random\ntext.Hello Lorem ipsum\nsome random\ntext.Hello Lorem ipsum\nsome random\ntext.Hello Lorem ipsum\nsome random\ntext.Hello Lorem ipsum\nsome random\ntext.Hello Lorem ipsum\nsome random\ntext.Hello Lorem ipsum\nsome random\ntext.Hello Lorem ipsum\nsome random\ntext.Hello Lorem ipsum\nsome random\ntext.Hello Lorem ipsum\nsome random\ntext.Hello Lorem ipsum\nsome random\ntext.Hello Lorem ipsum\nsome random\ntext.Hello Lorem ipsum\nsome random\ntext.Hello Lorem ipsum\nsome random\ntext.Hello Lorem ipsum\nsome random\ntext.Hello Lorem ipsum\nsome random\ntext.Hello Lorem ipsum\nsome random\ntext.Hello Lorem ipsum\nsome random\ntext.Hello Lorem ipsum\nsome random\ntext.Hello Lorem ipsum\nsome random\ntext.Hello Lorem ipsum\nsome random\ntext.Hello Lorem ipsum\nsome random\ntext.Hello Lorem ipsum\nsome random\ntext.Hello Lorem ipsum\nsome random\ntext.Hello Lorem ipsum\nsome random\ntext.Hello Lorem ipsum\nsome random\ntext.Hello Lorem ipsum\nsome random\ntext.Hello Lorem ipsum\nsome random\ntext.Hello Lorem ipsum\nsome random\ntext.Hello Lorem ipsum\nsome random\ntext.Hello Lorem ipsum\nsome random\ntext.Hello Lorem ipsum\nsome random\ntext.Hello Lorem ipsum\nsome random\ntext.Hello Lorem ipsum\nsome random\ntext.Hello Lorem ipsum\nsome random\ntext.Hello Lorem ipsum\nsome random\ntext.Hello Lorem ipsum\nsome random\ntext.Hello Lorem ipsum\nsome random\ntext.Hello Lorem ipsum\nsome random\ntext.Hello Lorem ipsum\nsome random\ntext.Hello Lorem ipsum\nsome random\ntext.Hello Lorem ipsum\nsome random\ntext.Hello Lorem ipsum\nsome random\ntext.Hello Lorem ipsum\nsome random\ntext.Hello Lorem ipsum\nsome random\ntext.Hello Lorem ipsum\nsome random\ntext.Hello Lorem ipsum\nsome random\ntext.Hello Lorem ipsum\nsome random\ntext.Hello Lorem ipsum\nsome random\ntext.Hello Lorem ipsum\nsome random\ntext.Hello Lorem ipsum\nsome random\ntext.Hello Lorem ipsum\nsome random\ntext.Hello Lorem ipsum\nsome random\ntext.Hello Lorem ipsum\nsome random\ntext.Hello Lorem ipsum\nsome random\ntext.Hello Lorem ipsum\nsome random\ntext.Hello Lorem ipsum\nsome random\ntext.Hello Lorem ipsum\nsome random\ntext.Hello Lorem ipsum\nsome random\ntext.Hello Lorem ipsum\nsome random\ntext.Hello Lorem ipsum\nsome random\ntext.Hello Lorem ipsum\nsome random\ntext.Hello Lorem ipsum\nsome random\ntext.Hello Lorem ipsum\nsome random\ntext.Hello Lorem ipsum\nsome random\ntext.Hello Lorem ipsum\nsome random\ntext.Hello Lorem ipsum\nsome random\ntext.Hello Lorem ipsum\nsome random\ntext.Hello Lorem ipsum\nsome random\ntext.Hello Lorem ipsum\nsome random\ntext.Hello Lorem ipsum\nsome random\ntext.Hello Lorem ipsum\nsome random\ntext.Hello Lorem ipsum\nsome random\ntext.Hello Lorem ipsum\nsome random\ntext.Hello Lorem ipsum\nsome random\ntext.Hello Lorem ipsum\nsome random\ntext.Hello Lorem ipsum\nsome random\ntext.Hello Lorem ipsum\nsome random\ntext.Hello Lorem ipsum\nsome random\ntext.Hello Lorem ipsum\nsome random\ntext.Hello Lorem ipsum\nsome random\ntext.Hello Lorem ipsum\nsome random\ntext.Hello Lorem ipsum\nsome random\ntext.Hello Lorem ipsum\nsome random\ntext.Hello Lorem ipsum\nsome random\ntext.Hello Lorem ipsum\nsome random\ntext.Hello Lorem ipsum\nsome random\ntext.Hello Lorem ipsum\nsome random\ntext.Hello Lorem ipsum\nsome random\ntext.Hello Lorem ipsum\nsome random\ntext.Hello Lorem ipsum\nsome random\ntext.Hello Lorem ipsum\nsome random\ntext.Hello Lorem ipsum\nsome random\ntext.Hello Lorem ipsum\nsome random\ntext.Hello Lorem ipsum\nsome random\ntext.Hello Lorem ipsum\nsome random\ntext.Hello Lorem ipsum\nsome random\ntext.Hello Lorem ipsum\nsome random\ntext.Hello Lorem ipsum\nsome random\ntext.Hello Lorem ipsum\nsome random\ntext.Hello Lorem ipsum\nsome random\ntext.Hello Lorem ipsum\nsome random\ntext.Hello Lorem ipsum\nsome random\ntext.Hello Lorem ipsum\nsome random\ntext.Hello Lorem ipsum\nsome random\ntext.Hello Lorem ipsum\nsome random\ntext.Hello Lorem ipsum\nsome random\ntext.Hello Lorem ipsum\nsome random\ntext.Hello Lorem ipsum\nsome random\ntext.Hello Lorem ipsum\nsome random\ntext.Hello Lorem ipsum\nsome random\ntext.Hello Lorem ipsum\nsome random\ntext.Hello Lorem ipsum\nsome random\ntext.Hello Lorem ipsum\nsome random\ntext.Hello Lorem ipsum\nsome random\ntext.Hello Lorem ipsum\nsome random\ntext.Hello Lorem ipsum\nsome random\ntext.Hello Lorem ipsum\nsome random\ntext.Hello Lorem ipsum\nsome random\ntext.Hello Lorem ipsum\nsome random\ntext.Hello Lorem ipsum\nsome random\ntext.Hello Lorem ipsum\nsome random\ntext.Hello Lorem ipsum\nsome random\ntext.Hello Lorem ipsum\nsome random\ntext.Hello Lorem ipsum\nsome random\ntext.Hello Lorem ipsum\nsome random\ntext.Hello Lorem ipsum\nsome random\ntext.Hello Lorem ipsum\nsome random\ntext.Hello Lorem ipsum\nsome random\ntext.Hello Lorem ipsum\nsome random\ntext.Hello Lorem ipsum\nsome random\ntext.Hello Lorem ipsum\nsome random\ntext.Hello Lorem ipsum\nsome random\ntext.Hello Lorem ipsum\nsome random\ntext.Hello Lorem ipsum\nsome random\ntext.Hello Lorem ipsum\nsome random\ntext.Hello Lorem ipsum\nsome random\ntext.Hello Lorem ipsum\nsome random\ntext.Hello Lorem ipsum\nsome random\ntext.Hello Lorem ipsum\nsome random\ntext.Hello Lorem ipsum\nsome random\ntext.Hello Lorem ipsum\nsome random\ntext.Hello Lorem ipsum\nsome random\ntext.Hello Lorem ipsum\nsome random\ntext.Hello Lorem ipsum\nsome random\ntext.Hello Lorem ipsum\nsome random\ntext.Hello Lorem ipsum\nsome random\ntext.Hello Lorem ipsum\nsome random\ntext.Hello Lorem ipsum\nsome random\ntext.Hello Lorem ipsum\nsome random\ntext.Hello Lorem ipsum\nsome random\ntext.Hello Lorem ipsum\nsome random\ntext.Hello Lorem ipsum\nsome random\ntext.Hello Lorem ipsum\nsome random\ntext.Hello Lorem ipsum\nsome random\ntext.Hello Lorem ipsum\nsome random\ntext.Hello Lorem ipsum\nsome random\ntext.Hello Lorem ipsum\nsome random\ntext.Hello Lorem ipsum\nsome random\ntext.Hello Lorem ipsum\nsome random\ntext.Hello Lorem ipsum\nsome random\ntext.Hello Lorem ipsum\nsome random\ntext.Hello Lorem ipsum\nsome random\ntext.Hello Lorem ipsum\nsome random\ntext.Hello Lorem ipsum\nsome random\ntext.Hello Lorem ipsum\nsome random\ntext.Hello Lorem ipsum\nsome random\ntext.Hello Lorem ipsum\nsome random\ntext.Hello Lorem ipsum\nsome random\ntext.Hello Lorem ipsum\nsome random\ntext.Hello Lorem ipsum\nsome random\ntext.Hello Lorem ipsum\nsome random\ntext.Hello Lorem ipsum\nsome random\ntext.Hello Lorem ipsum\nsome random\ntext.Hello Lorem ipsum\nsome random\ntext.Hello Lorem ipsum\nsome random\ntext.Hello Lorem ipsum\nsome random\ntext.Hello Lorem ipsum\nsome random\ntext.Hello Lorem ipsum\nsome random\ntext.Hello Lorem ipsum\nsome random\ntext.Hello Lorem ipsum\nsome random\ntext.Hello Lorem ipsum\nsome random\ntext.Hello Lorem ipsum\nsome random\ntext.Hello Lorem ipsum\nsome random\ntext.Hello Lorem ipsum\nsome random\ntext.Hello Lorem ipsum\nsome random\ntext.Hello Lorem ipsum\nsome random\ntext.Hello Lorem ipsum\nsome random\ntext.Hello Lorem ipsum\nsome random\ntext.Hello Lorem ipsum\nsome random\ntext.Hello Lorem ipsum\nsome random\ntext.Hello Lorem ipsum\nsome random\ntext.Hello Lorem ipsum\nsome random\ntext.Hello Lorem ipsum\nsome random\ntext.Hello Lorem ipsum\nsome random\ntext.Hello Lorem ipsum\nsome random\ntext.Hello Lorem ipsum\nsome random\ntext.Hello Lorem ipsum\nsome random\ntext.Hello Lorem ipsum\nsome random\ntext.Hello Lorem ipsum\nsome random\ntext.Hello Lorem ipsum\nsome random\ntext.Hello Lorem ipsum\nsome random\ntext.Hello Lorem ipsum\nsome random\ntext.Hello Lorem ipsum\nsome random\ntext.Hello Lorem ipsum\nsome random\ntext.Hello Lorem ipsum\nsome random\ntext.Hello Lorem ipsum\nsome random\ntext.Hello Lorem ipsum\nsome random\ntext.Hello Lorem ipsum\nsome random\ntext.Hello Lorem ipsum\nsome random\ntext.Hello Lorem ipsum\nsome random\ntext.Hello Lorem ipsum\nsome random\ntext.Hello Lorem ipsum\nsome random\ntext.Hello Lorem ipsum\nsome random\ntext.Hello Lorem ipsum\nsome random\ntext.Hello Lorem ipsum\nsome random\ntext.Hello Lorem ipsum\nsome random\ntext.Hello Lorem ipsum\nsome random\ntext.Hello Lorem ipsum\nsome random\ntext.Hello Lorem ipsum\nsome random\ntext.Hello Lorem ipsum\nsome random\ntext.Hello Lorem ipsum\nsome random\ntext.Hello Lorem ipsum\nsome random\ntext.Hello Lorem ipsum\nsome random\ntext.Hello Lorem ipsum\nsome random\ntext.Hello Lorem ipsum\nsome random\ntext.Hello Lorem ipsum\nsome random\ntext.Hello Lorem ipsum\nsome random\ntext.Hello Lorem ipsum\nsome random\ntext.Hello Lorem ipsum\nsome random\ntext.Hello Lorem ipsum\nsome random\ntext.Hello Lorem ipsum\nsome random\ntext.Hello Lorem ipsum\nsome random\ntext.Hello Lorem ipsum\nsome random\ntext.Hello Lorem ipsum\nsome random\ntext.Hello Lorem ipsum\nsome random\ntext.Hello Lorem ipsum\nsome random\ntext.Hello Lorem ipsum\nsome random\ntext.Hello Lorem ipsum\nsome random\ntext.Hello Lorem ipsum\nsome random\ntext.Hello Lorem ipsum\nsome random\ntext.Hello Lorem ipsum\nsome random\ntext.Hello Lorem ipsum\nsome random\ntext.Hello Lorem ipsum\nsome random\ntext.Hello Lorem ipsum\nsome random\ntext.Hello Lorem ipsum\nsome random\ntext.Hello Lorem ipsum\nsome random\ntext.Hello Lorem ipsum\nsome random\ntext.Hello Lorem ipsum\nsome random\ntext.Hello Lorem ipsum\nsome random\ntext.Hello Lorem ipsum\nsome random\ntext.Hello Lorem ipsum\nsome random\ntext.Hello Lorem ipsum\nsome random\ntext.Hello Lorem ipsum\nsome random\ntext.Hello Lorem ipsum\nsome random\ntext.Hello Lorem ipsum\nsome random\ntext.Hello Lorem ipsum\nsome random\ntext.Hello Lorem ipsum\nsome random\ntext.Hello Lorem ipsum\nsome random\ntext.";
        //std::string msgToSend = "Hello Lorem ipsumHello Lorem ipsumHello Lorem ipsumHello Lorem ipsumHello Lorem ipsumHello Lorem ipsumHello Lorem ipsumHello Lorem ipsumHello Lorem ipsumHello Lorem ipsumHello Lorem ipsumHello Lorem ipsumHello Lorem ipsumHello Lorem ipsumHello Lorem ipsumHello Lorem ipsumHello Lorem ipsumEND";
        //std::string msgToSend = "Hello.";
        //send_msg(msgToSend);
        //while(true);
        start_read();
    }catch(std::exception& e){
        std::cerr << "Client err: " << e.what() << std::endl;
    }
}


// TODO replace binds with std::function

void Client::start_read()
{
    asio::async_read(socket_, asio::buffer(&msgLength, sizeof(msgLength)), asio::transfer_exactly(sizeof(msgLength)),
                     std::bind(&Client::handle_read_msg_size, this, std::placeholders::_1, std::placeholders::_2));
}

void Client::handle_read_msg_size(const asio::error_code& error,
                                         size_t bytes_transferred)
{
    if(error == asio::error::eof){
        std::cerr << "Client err: " << error.message() << " (maybe connection closed?)";
        return;
    }
    if(!error){
        std::cout << "Client Msg size: " << msgLength << std::endl;
        msgBuffer_ = std::make_shared<asio::streambuf>(msgLength);
        read_msg_content();
    }else{
        throw std::runtime_error("Client err in reading msg size: " + error.message());
    }
}

void Client::read_msg_content(){
    int restToRead = msgLength - alreadyRead_;
    if(restToRead >MAX_PACKET_SIZE){
        asio::async_read(socket_, *msgBuffer_,asio::transfer_exactly(MAX_PACKET_SIZE),
                         std::bind(&Client::handle_read_msg_content, this, std::placeholders::_1, std::placeholders::_2));
    }else{
        asio::async_read(socket_, *msgBuffer_,asio::transfer_exactly(restToRead),
                         std::bind(&Client::handle_read_msg_content, this, std::placeholders::_1, std::placeholders::_2));
    }
}

void Client::handle_read_msg_content(const asio::error_code &error, size_t bytes_transferred) {
    if(!error){
        alreadyRead_ += bytes_transferred;
        std::cout << "Client content bytes read: " << alreadyRead_ << "/" << msgLength <<std::endl;
        std::istream istream(&(*msgBuffer_));
        std::string msgPart(std::istreambuf_iterator<char>(istream), {});
        finalMessage_ += msgPart;
        msgBuffer_->consume(bytes_transferred);
        if(alreadyRead_ != msgLength){
            read_msg_content();
        }else{
            alreadyRead_ = 0;
            messagesVector_->push_back(finalMessage_);
            std::cout << "Client received: " << finalMessage_ << std::endl;
            finalMessage_ = "";
            start_read();
        }
    }else{
        throw std::runtime_error("Client Error in reading msg content: " + error.message());
    }
}

void Client::send_msg(std::string &msg) { //TODO client send should be async
    asio::error_code errorWrite;
    uint32_t  msgSize = msg.length();
    asio::write(socket_, asio::buffer(&msgSize, sizeof(msgSize)), errorWrite);
    asio::write(socket_, asio::buffer(msg), errorWrite);
    if(errorWrite){
        throw std::runtime_error("Error in client write: " + errorWrite.message());
    }
}

std::shared_ptr<std::vector<std::string>> Client::getMessagesVector() {
    return messagesVector_;
}

std::string Client::getFrontMessage() {
    return messagesVector_->front();
}

std::string Client::popMessage() {
    auto msgToReturn = getFrontMessage();
    messagesVector_->erase(messagesVector_->begin());
    return msgToReturn;
}
