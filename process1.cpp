#include <iostream>
#include<unistd.h>
#include <csignal>
#include <cstring>
#include <asio.hpp>
void signal_handler(int signum)
{
    std::cout << "Caught signal "<< signum << std::endl;
}

int main(){
    try {
        asio::io_context io_context;
        asio::ip::tcp::socket socket(io_context);
        asio::ip::tcp::endpoint endpoint(asio::ip::address::from_string("127.0.0.1"), 13);
        socket.connect(endpoint);
        int counter = 0;
        for (;;)
        {
            counter ++;
            if(counter == 3){
                asio::error_code errorWrite;
                std::cout << "sending message..." << std::endl;
                asio::write(socket, asio::buffer(std::string("Some message from client...\n")), errorWrite);
                std::cout << "err write: " << errorWrite << std::endl;
            }
            asio::error_code error;
            std::vector<char> buf(128);
            size_t len = socket.read_some(asio::buffer(buf), error);

            if (error == asio::error::eof)
                break; // Connection closed cleanly by peer.
            else if (error)
                throw asio::system_error(error); // Some other error.
            std::cout << "DATA: " << buf.data() << std::endl;
        }
    }
    catch (std::exception& e) {
        std::cerr << "Exception: " << e.what() << std::endl;
    }
    /*
    struct sigaction action{};
    memset(&action, 0, sizeof(action));
    action.sa_handler = signal_handler;
    sigaction(SIGTERM, &action, nullptr);
    sigaction(SIGINT, &action, nullptr);


    int i = 0;
    while(true){
        i++;
        unsigned int microsecond = 1000000;
        usleep(1 * microsecond);//sleeps for 1 second
        std::cout << "hello" << i << std::endl;
    }
     */
}

