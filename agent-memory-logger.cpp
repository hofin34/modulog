#include <iostream>
#include<unistd.h>
#include <csignal>
#include <cstring>
#include <asio.hpp>
#include "ControlMessage.h"
#include "MessageSerializer.h"
#include "MessageDeserializer.h"
#include "TcpConnection.h"
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

void sendLog(TcpConnection::pointer& connection){
    std::string usedMemory =  exec("free | awk 'FNR == 2 { print $3 }'");
    auto logMessage = std::make_shared<LogMessage>(LogMessage::LOG_MSG_TYPE::LOG, usedMemory);
    MessageSerializer messageSerializer(logMessage);
    std::string toSend = messageSerializer.serialize();
    connection->send_message(toSend);
}

void startSendLogs(asio::steady_timer* timer, TcpConnection::pointer& connection){
    sendLog(connection);
    timer->expires_from_now(std::chrono::seconds(3));
    timer->async_wait(std::bind(startSendLogs,timer, connection));
}

int main(){

    try {
        asio::io_context io_context;
        std::string agentName = "agent";
        TcpConnection::pointer connection = TcpConnection::create(io_context, agentName);
        asio::ip::tcp::endpoint endpoint(asio::ip::address::from_string("127.0.0.1"), 1234);
        connection->get_socket().connect(endpoint);
        connection->start_read();
        //std::string msg = "XXX";
       // connection->send_message(msg);
        std::thread clientThread{[&io_context]() { io_context.run(); }};

        std::shared_ptr<std::string> configMsgString;
        while ((configMsgString = connection->popMessage()) == nullptr);
        std::cout << "Process received: " << *configMsgString << std::endl;
        // received config
        MessageDeserializer messageDeserializer(*configMsgString);
        if(messageDeserializer.getMsgType() == Message::MSG_TYPE::CONTROL_MSG){
            auto configMessage = messageDeserializer.getControlMessage();
            nlohmann::json configJson = nlohmann::json::parse(configMessage->getValue());
            //TODO parse json
            std::string agentId = configJson["id"];
            auto ackMessage = std::make_shared<ControlMessage>(ControlMessage::CONTROL_MSG_TYPE::ACK, agentId);
            MessageSerializer msgSerializer(ackMessage);
            std::string initResponse = msgSerializer.serialize();
            connection->send_message(initResponse);
        }
        asio::steady_timer sendingTimer(io_context);
        startSendLogs(&sendingTimer, connection);

        while(true){
            std::shared_ptr<std::string> messageString;
            while ((messageString = connection->popMessage()) == nullptr);
            MessageDeserializer messageDeserializer1(*messageString);
            if(messageDeserializer1.getMsgType() == Message::MSG_TYPE::CONTROL_MSG){
                std::shared_ptr<ControlMessage> controlMessage = messageDeserializer1.getControlMessage();
                if(controlMessage->getType() == ControlMessage::CONTROL_MSG_TYPE::IS_ALIVE){ // Response to isAlive
                    auto ackAliveMsg = std::make_shared<ControlMessage>(ControlMessage::CONTROL_MSG_TYPE::ACK, "");
                    MessageSerializer messageSerializer(ackAliveMsg);
                    std::string toSend = messageSerializer.serialize();
                    connection->send_message(toSend);
                }
            }
        }
        clientThread.join();
    }catch(std::exception& e){
        std::cerr << e.what() << std::endl;
    }
        return 0;
}

