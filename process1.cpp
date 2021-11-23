#include <iostream>
#include<unistd.h>
#include <csignal>
#include <cstring>
#include <asio.hpp>
#include "Client.h"
#include <thread>
void signal_handler(int signum)
{
    std::cout << "Caught signal "<< signum << std::endl;
}

int main(){

    try {
        asio::io_context io_context;
        auto client = std::make_shared<Client>(io_context);
        std::thread clientThread{[&io_context]() { io_context.run(); }};
        while (client->getMessagesVector()->empty());
        auto initMsg = client->getFrontMessage();
        std::cout << "in: " << initMsg << std::endl;
        std::string initResponse = "response to init msg";
        client->send_msg(initResponse);
        clientThread.join();
    }catch(std::exception& e){
        std::cerr << e.what() << std::endl;
    }
        return 0;
}

