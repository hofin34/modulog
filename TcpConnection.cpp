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

void read_msg_size(){

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
    //asio::async_read(socket_, asio::buffer(&msgLength, sizeof(msgLength)),
       //                    std::bind(&TcpConnection::handle_read_msg_size, shared_from_this(), std::placeholders::_1, std::placeholders::_2));

    return;
       asio::error_code readErr;
    std::cout << "Starting read msg size..." << std::endl;
    uint32_t  msgSize;
    socket_.read_some(asio::buffer(&msgSize, sizeof(msgSize)), readErr);
    std::vector<char> buf(msgSize);
    std::cout << "Starting read msg data..." << std::endl;
    socket_.read_some(asio::buffer(buf), readErr);
    if (readErr == asio::error::eof)
        return; // Connection closed cleanly by peer.
    else if (readErr)
        throw asio::system_error(readErr); // Some other error.
    std::cout << "DATA: " << buf.data() << std::endl;

}

void TcpConnection::handle_read_msg_size(const asio::error_code& error,
                                         size_t bytes_transferred)
{
    std::cout << "Msg size: " << msgLength;
    lastMessageBuffer = std::vector<char>(msgLength);
    asio::async_read(socket_, asio::buffer(lastMessageBuffer, msgLength),
                            std::bind(&TcpConnection::handle_read_msg_content, this, std::placeholders::_1, std::placeholders::_2));
    std::cout << "Read err_code: " << error << " Msg:  " << error.message() << std::endl;

    std::cout << "Rec data: " << lastMessageBuffer.data() << std::endl; //TODO if isAliveAck -> isWaiting = false
    start_read();
}

void TcpConnection::handle_read_msg_content(const asio::error_code &error, size_t bytes_transferred) {

}