#include "Client.h"
#include "MessageDeserializer.h"
#include "MessageSerializer.h"

Client::Client(asio::io_context& io_context) : socket_(io_context), ioContext_(io_context){
    try{
        asio::ip::tcp::endpoint endpoint(asio::ip::address::from_string("127.0.0.1"), 1234);
        socket_.connect(endpoint);

        asio::error_code errorWrite;
        std::string msgToSend = "Hello Lorem ipsum some random text.\n";
        uint32_t  msgSize = msgToSend.length();
        std::cout << "sending message: " << msgToSend << std::endl;
        asio::write(socket_, asio::buffer(&msgSize, sizeof(msgSize)), errorWrite);
        asio::write(socket_, asio::buffer(msgToSend), errorWrite);
        std::cout << errorWrite << std::endl;
        return;
        start_read();
        std::thread thread1{[&io_context](){ io_context.run(); }};
        //std::thread thread2{[&io_context](){ io_context.run(); }};
        log();

        thread1.join();
        //thread2.join();
    }catch(std::exception& e){
        std::cerr << "Err: " << e.what() << std::endl;
    }
}

void Client::send_log(std::string& msg) {
    std::cout << "sending message..." << std::endl;
    start_write(msg);
}

// TODO replace binds with std::function
void Client::start_write(std::string& msg)
{
    std::cout << "start_write()" << std::endl;
    msg_to_send_ = msg;
    asio::async_write(socket_, asio::buffer(msg),
                      std::bind(&Client::handle_write, this,
                                std::placeholders::_1, // error
                                std::placeholders::_2)); // bytes_transfered
}
void Client::handle_write(const asio::error_code& error,
                  size_t bytes_transferred)
{
    if(error.value() != 0){ // client disconnected
        std::cerr << "Some error in write: " << error.message() << std::endl;
    }else{
        std::cout << "Transferedx " << bytes_transferred << " B." << std::endl;
    }
}

void Client::log() {
    for(int i = 0;;i++){ //logging something...
        std::cout << "sleeping..." << std::endl;
        std::this_thread::sleep_for(std::chrono::milliseconds(5000));
        std::string msg = "Client counter msg: " + std::to_string(i) + "\n";
        std::cout << "Sending: " << msg << std::endl;
       // send_log(msg);
    }
}

void Client::start_read() {
    std::cout << "start_read()" << std::endl;
    asio::async_read(socket_, asio::buffer(&msgLength, sizeof(msgLength)),
                     std::bind(&Client::handle_read_msg_size, this, std::placeholders::_1, std::placeholders::_2) );
}

void Client::handle_read_msg_size(const asio::error_code &error, size_t bytes_transferred) {
    std::cout << "Msg size: " << msgLength;
    // start read real content:
    asio::error_code content_read_err;
    std::vector<char> buf(msgLength);
    size_t len = socket_.read_some(asio::buffer(buf), content_read_err);
    if(len != msgLength){
        std::cerr << "!!!WARNING!!!" << std::endl << "Received different number of bytes than header said!" << std::endl;
    }
    if (content_read_err == asio::error::eof)
        return; // Connection closed cleanly by peer.
    else if (content_read_err)
        throw asio::system_error(error); // Some other error.
    std::cout << "DATA: " << buf.data() << std::endl;

    /*
    auto controlMsg = std::make_shared<ControlMessage>(ControlMessage::CONTROL_MSG_TYPE::ACK, "Is Alive ACK...");
    MessageSerializer messageSerializer(controlMsg);
    std::string msgToSend = messageSerializer.serialize();
    start_write(msgToSend);
    */

}

void Client::handle_read_content(const asio::error_code &error, size_t bytes_transferred) {
    std::cout << "Read err_code: " << error << " Msg:  " << error.message() << std::endl;
    std::string line;
    std::istream is(&input_buffer_);
    std::getline(is, line);
    MessageDeserializer messageDeserializer(line);
    if(messageDeserializer.getMsgType() == Message::MSG_TYPE::CONTROL_MSG){
        if(messageDeserializer.getControlMessage()->getType() == ControlMessage::CONTROL_MSG_TYPE::IS_ALIVE)
        {
            //TODO response with ACK
        }
    }

    std::cout << "Rec data: " << line << std::endl;
    start_read();
}