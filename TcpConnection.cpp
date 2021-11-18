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
    // Start an asynchronous operation to read a newline-delimited message.
    asio::async_read(socket_, asio::buffer(&msgLength, sizeof(msgLength)),
                           std::bind(&TcpConnection::handle_read_msg_size, shared_from_this(), std::placeholders::_1, std::placeholders::_2));
}

void TcpConnection::handle_read_msg_size(const asio::error_code& error,
                                         size_t bytes_transferred)
{
    std::cout << "Transfered: " << bytes_transferred << std::endl;
    std::cout << "Msg size: " << msgLength << std::endl;

    // https://www.boost.org/doc/libs/1_40_0/doc/html/boost_asio/example/http/client/async_client.cpp
    asio::async_read(socket_, asio::dynamic_buffer(lastMessageBuffer, msgLength),
                     [&] (asio::error_code error, std::size_t bytes_transferred){
        std::cout << error.message() << ", transf.:" << bytes_transferred << "\n";
    });

    //lastMessageBuffer = std::vector<char>(msgLength);

    //asio::async_read(socket_,  asio::buffer(lastMessageBuffer, msgLength),
     //                       std::bind(&TcpConnection::handle_read_msg_content, this, std::placeholders::_1, std::placeholders::_2));
    //std::cout << "Read err_code: " << error << " Msg:  " << error.message() << std::endl;
}

void TcpConnection::handle_read_msg_content(const asio::error_code &error, size_t bytes_transferred) {
    std::cout << "Err:" << error.message() << std::endl;
    std::cout << "Cont. transfered: " << bytes_transferred << std::endl;
    std::cout << "Receiving msg content: " << lastMessageBuffer.data() << std::endl;
    // TODO save msg to messages buffer
    //start_read();
}