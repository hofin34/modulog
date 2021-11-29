#include <iostream>
#include<unistd.h>
#include <csignal>
#include <cstring>
#include <asio.hpp>
#include "../../communication/include/ControlMessage.h"
#include "../../communication/include/MessageSerializer.h"
#include "../../communication/include/MessageDeserializer.h"
#include "../../communication/include/TcpConnection.h"
#include "../../communication/include/AgentClient.h"
#include <thread>
void signal_handler(int signum)
{
    std::cout << "Caught signal "<< signum << std::endl;
}


std::string getUsedRAM(){
    std::string usedMemory =  AgentClient::execCommand("free | awk 'FNR == 2 { print $3 }'");
    usedMemory.erase(std::remove(usedMemory.begin(), usedMemory.end(), '\n'), usedMemory.end()); // remove newline
    return usedMemory;
}


int main(){
    auto ioContext = std::make_shared<asio::io_context>();
    AgentClient agentClient(ioContext, false, "agent-memory-logger" );
    while(true){
        auto logMsg = std::make_shared<LogMessage>(LogMessage::LOG_MSG_TYPE::LOG, "usedRAM", getUsedRAM());
        std::cout << "Ag sending: "  << std::endl;
        agentClient.sendLog(logMsg);
        auto logMsg2 = std::make_shared<LogMessage>(LogMessage::LOG_MSG_TYPE::LOG, "freeSpace", "5");
        agentClient.sendLog(logMsg2);
        std::this_thread::sleep_for(std::chrono::seconds(2));
    }

    return 0;
}
