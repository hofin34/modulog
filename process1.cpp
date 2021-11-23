#include <iostream>
#include<unistd.h>
#include <csignal>
#include <cstring>
#include <asio.hpp>
#include "Client.h"
#include "ControlMessage.h"
#include "MessageSerializer.h"
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
        std::cout << "Process received: " << initMsg << std::endl;
        // received config
        auto ackMessage = std::make_shared<ControlMessage>(ControlMessage::CONTROL_MSG_TYPE::ACK, "process1Name");
        MessageSerializer msgSerializer(ackMessage);
        std::string initResponse = msgSerializer.serialize();
        client->send_msg(initResponse);
        clientThread.join();
    }catch(std::exception& e){
        std::cerr << e.what() << std::endl;
    }
        return 0;
}

