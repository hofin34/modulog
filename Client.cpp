#include "Client.h"
#include "MessageHandler.h"

Client::Client(asio::io_context& io_context) : socket_(io_context), ioContext_(io_context){
    try{
        asio::ip::tcp::endpoint endpoint(asio::ip::address::from_string("127.0.0.1"), 13);
        socket_.connect(endpoint);
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
    asio::async_read_until(socket_, input_buffer_, '\n',
                           std::bind(&Client::handle_read, this, std::placeholders::_1, std::placeholders::_2)); //TODO should be shared_from_this instead of this?
}

void Client::handle_read(const asio::error_code &error, size_t bytes_transferred) {
    std::cout << "Read err_code: " << error << " Msg:  " << error.message() << std::endl;
    std::string line;
    std::istream is(&input_buffer_);
    std::getline(is, line);
    MessageHandler msgHandler(line);
    if(msgHandler.getMessageType() == MessageHandler::msg_type::CONTROL){
        if(msgHandler.getControlMessage().getType() == ControlMessage::IS_ALIVE){
            //Response with ACK...
        }
    }
    std::cout << "Rec data: " << line << std::endl;
    start_read();
}