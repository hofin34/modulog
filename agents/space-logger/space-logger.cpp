#include <iostream>
#include <asio.hpp>
#include "../../communication/include/AgentClient.h"
#include <thread>
#include <filesystem>


int main(int argc, char** argv){
    nlohmann::json configJson = AgentClient::parseConfig(argv[0]);
    if(!configJson.contains("id") || !configJson.contains("folderToMonitor")){
        std::cerr << "You must specify id and folderToMonitor in config." << std::endl;
        exit(EXIT_FAILURE);
    }
    int logInterval = 10;
    if(configJson.contains("logInterval"))
        logInterval = configJson["logInterval"];
    std::filesystem::space_info spaceInfo = std::filesystem::space(configJson["folderToMonitor"]);
    auto ioContext = std::make_shared<asio::io_context>();
    AgentClient agentClient(ioContext, false, configJson["id"]);
    agentClient.initClient();
    while(true){
        auto logMsg2 = std::make_shared<LogMessage>(LogMessage::LOG_MSG_TYPE::LOG, "availableSpaceMB", std::to_string(spaceInfo.available/1024));
        agentClient.sendLog(logMsg2);
        auto logMsg3 = std::make_shared<LogMessage>(LogMessage::LOG_MSG_TYPE::LOG, "freeSpaceMB", std::to_string(spaceInfo.free/1024));
        agentClient.sendLog(logMsg3);
        std::this_thread::sleep_for(std::chrono::seconds(logInterval));
    }
    return 0;
}
