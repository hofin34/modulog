#include "TcpConnection.h"


TcpConnection::pointer TcpConnection::create(asio::io_context& io_context)
{
    return pointer(new TcpConnection(io_context)); //TODO refactor creation...
}

asio::ip::tcp::socket& TcpConnection::get_socket()
{
    return socket_;
}

void TcpConnection::start_alive_writer(){
    aliveTimer_.expires_from_now(asio::chrono::seconds(4));
    aliveTimer_.async_wait(std::bind(&TcpConnection::send_alive, this));
}

void TcpConnection::send_alive(){
    asio::error_code errorWrite;
    auto aliveControlMsg = std::make_shared<ControlMessage>(ControlMessage::CONTROL_MSG_TYPE::IS_ALIVE, "") ;
    MessageSerializer messageSerializer(aliveControlMsg);
    std::string msgToSend = messageSerializer.serialize();
    uint32_t  msgSize = msgToSend.length();
    std::cout << "sending message: " << msgToSend << std::endl;
    asio::write(socket_, asio::buffer(&msgSize, sizeof(msgSize)), errorWrite);
    std::cout << "err write header: " << errorWrite.message() << std::endl;
    asio::write(socket_, asio::buffer(msgToSend), errorWrite);
    std::cout << "err write data: : " << errorWrite << std::endl;
    waitingForACKAlive = true;
    aliveResponseTimer_.expires_from_now(asio::chrono::seconds(2));
    aliveResponseTimer_.async_wait(std::bind(&TcpConnection::alive_handler, this));
    //start_alive_writer();
}


void TcpConnection::alive_handler(){ // if is saved response -> check number -> if correct -> start_alive_writer() else send_alive
    if(waitingForACKAlive){
        //kill / reset
    }else{
        std::cerr << "Client sends ACK when server not expecting..." << std::endl;
    }
}

void TcpConnection::start_read()
{
    asio::async_read(socket_, asio::buffer(&msgLength, sizeof(msgLength)),
                           std::bind(&TcpConnection::handle_read_msg_size, shared_from_this(), std::placeholders::_1, std::placeholders::_2));
}

void TcpConnection::handle_read_msg_size(const asio::error_code& error,
                                         size_t bytes_transferred)
{
    if(!error){
        std::cout << "Msg size: " << msgLength << std::endl;

        asio::async_read(socket_,  msgBuffer, asio::transfer_at_least(1),
                         std::bind(&TcpConnection::handle_read_msg_content, shared_from_this(), std::placeholders::_1, std::placeholders::_2));

    }else{
        throw std::runtime_error("Err in reading msg size: " + error.message());
    }
 }

void TcpConnection::handle_read_msg_content(const asio::error_code &error, size_t bytes_transferred) {
    if(!error){
        if(bytes_transferred != msgLength){
            std::cerr << "Transfered: " << bytes_transferred << std::endl;
            throw std::runtime_error("Transfered bytes is different than message length in header!");
        }
        std::istream istream(&msgBuffer);
        std::string s(std::istreambuf_iterator<char>(istream), {});
        messagesVector_.push_back(s);
    }else{
        throw std::runtime_error("Error in reading msg content: " + error.message());
    }
}