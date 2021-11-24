#include <iostream>
#include<unistd.h>
#include <csignal>
#include <cstring>
#include <asio.hpp>
#include "Client.h"
#include "ControlMessage.h"
#include "MessageSerializer.h"
#include "MessageDeserializer.h"
#include <thread>
void signal_handler(int signum)
{
    std::cout << "Caught signal "<< signum << std::endl;
}

std::string exec(const char* cmd) {
    std::array<char, 128> buffer;
    std::string result;
    std::unique_ptr<FILE, decltype(&pclose)> pipe(popen(cmd, "r"), pclose);
    if (!pipe) {
        throw std::runtime_error("popen() failed!");
    }
    while (fgets(buffer.data(), buffer.size(), pipe.get()) != nullptr) {
        result += buffer.data();
    }
    return result;
}

int main(){

    try {
        asio::io_context io_context;
        auto client = std::make_shared<Client>(io_context);
        std::thread clientThread{[&io_context]() { io_context.run(); }};

        while (client->getMessagesVector()->empty());
        auto firstMsg = client->getFrontMessage();
        std::cout << "Process received: " << firstMsg << std::endl;
        // received config
        MessageDeserializer messageDeserializer(firstMsg);
        if(messageDeserializer.getMsgType() == Message::MSG_TYPE::CONTROL_MSG){
            auto configMessage = messageDeserializer.getControlMessage();
            nlohmann::json configJson = nlohmann::json::parse(configMessage->getValue());
            //TODO parse json
            std::string agentId = configJson["id"];
            auto ackMessage = std::make_shared<ControlMessage>(ControlMessage::CONTROL_MSG_TYPE::ACK, agentId);
            MessageSerializer msgSerializer(ackMessage);
            std::string initResponse = msgSerializer.serialize();
            client->send_msg(initResponse);        }

        while(true){
            std::string usedMemory =  exec("free | awk 'FNR == 2 { print $3 }'");
            auto logMessage = std::make_shared<LogMessage>(LogMessage::LOG_MSG_TYPE::LOG, usedMemory);
            MessageSerializer messageSerializer(logMessage);
            std::string toSend = messageSerializer.serialize();
            client->send_msg(toSend);
            std::this_thread::sleep_for(std::chrono::seconds(2));
        }
        clientThread.join();
    }catch(std::exception& e){
        std::cerr << e.what() << std::endl;
    }
        return 0;
}

