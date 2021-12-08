#include <iostream>
#include <asio.hpp>
#include "../../communication/include/MessageSerializer.h"
#include "../../communication/include/AgentClient.h"
#include <thread>
#include <sys/statvfs.h>
#include <filesystem>


int main(){
    std::filesystem::space_info spaceInfo = std::filesystem::space("/");
    auto ioContext = std::make_shared<asio::io_context>();
    AgentClient agentClient(ioContext, false, "agent-space-logger" );
    agentClient.initClient();
    while(true){
        auto logMsg2 = std::make_shared<LogMessage>(LogMessage::LOG_MSG_TYPE::LOG, "availableSpaceMB", std::to_string(spaceInfo.available/1024));
        agentClient.sendLog(logMsg2);
        auto logMsg3 = std::make_shared<LogMessage>(LogMessage::LOG_MSG_TYPE::LOG, "freeSpaceMB", std::to_string(spaceInfo.free/1024));
        agentClient.sendLog(logMsg3);
        std::this_thread::sleep_for(std::chrono::seconds(2));
    }
    return 0;
}

