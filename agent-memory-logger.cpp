#include <iostream>
#include<unistd.h>
#include <csignal>
#include <cstring>
#include <asio.hpp>
#include "ControlMessage.h"
#include "MessageSerializer.h"
#include "MessageDeserializer.h"
#include "TcpConnection.h"
#include "AgentClient.h"
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

std::string getUsedRAM(){
    std::string usedMemory =  exec("free | awk 'FNR == 2 { print $3 }'");
    usedMemory.erase(std::remove(usedMemory.begin(), usedMemory.end(), '\n'), usedMemory.end()); // remove newline
    return usedMemory;
}


int main(){
    auto ioContext = std::make_shared<asio::io_context>();
    AgentClient agentClient(ioContext, false, "agent-memory-logger" );
    while(true){
        std::cout <<"ST" << std::endl;
        auto logMsg = std::make_shared<LogMessage>(LogMessage::LOG_MSG_TYPE::LOG, "usedRAM", getUsedRAM());
        std::cout << "Ag sending: "  << std::endl;
        agentClient.sendLog(logMsg);
        std::cout << "sent." << std::endl;
        std::this_thread::sleep_for(std::chrono::seconds(2));
    }

    return 0;
}

