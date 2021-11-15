#include <iostream>
#include "TcpConnection.h"
#include "MessageSerializer.h"

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
    std::cout << "sending message..." << std::endl;
    auto aliveControlMsg = std::make_shared<ControlMessage>(ControlMessage::CONTOL_MSG_TYPE::IS_ALIVE, "") ; //TODO end message with special character.
    MessageSerializer messageSerializer(aliveControlMsg);
    std::string msgToSend = messageSerializer.serialize();
    uint32_t  msgSize = msgToSend.length();
    asio::write(socket_, asio::buffer(&msgSize, sizeof(msgSize)), errorWrite);
    asio::write(socket_, asio::buffer(msgToSend), errorWrite);
    std::cout << "err write: " << errorWrite << std::endl;
    waitingForACKAlive = true;
    aliveResponseTimer_.expires_from_now(asio::chrono::seconds(2));
    aliveResponseTimer_.async_wait(std::bind(&TcpConnection::alive_handler, this));
}

void TcpConnection::alive_handler(){ // if is saved response -> check number -> if correct -> start_alive_writer() else send_alive
    if(waitingForACKAlive){
        //kill / reset
    }
}

void TcpConnection::start_read()
{
    // Start an asynchronous operation to read a newline-delimited message.
    asio::async_read_until(socket_, input_buffer_, '\n',
                           std::bind(&TcpConnection::handle_read, shared_from_this(), std::placeholders::_1, std::placeholders::_2));
}

void TcpConnection::handle_read(const asio::error_code& error,
                 size_t bytes_transferred)
{
    std::cout << "Read err_code: " << error << " Msg:  " << error.message() << std::endl;
    std::string line;
    std::istream is(&input_buffer_);
    std::getline(is, line);
    std::cout << "Rec data: " << line << std::endl; //TODO if isAliveAck -> isWaiting = false
    start_read();
}