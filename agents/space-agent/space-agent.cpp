/*
 * modulog agent, that monitors disk space (look at README.md for more info)
 */

#include <modulog/agent_client/AgentClient.hpp>
#include <modulog/agent_client/Helpers.hpp>

#include <thread>
#include <filesystem>
#include <iostream>


int main(int argc, char** argv){
    nlohmann::json configJson = modulog::agent_client::Helpers::parseConfig(argv[0]);
    if(!configJson.contains("id") || !configJson.contains("folderToMonitor")){
        std::cerr << "You must specify id and folderToMonitor in config." << std::endl;
        exit(EXIT_FAILURE);
    }
    int logInterval = 10;
    if(configJson.contains("logInterval"))
        logInterval = configJson["logInterval"];
    std::filesystem::space_info spaceInfo = std::filesystem::space(configJson["folderToMonitor"]);
    auto ioContext = std::make_shared<asio::io_context>();
    modulog::agent_client::AgentClient agentClient(ioContext, false, configJson["id"]);
    agentClient.initClient();
    while(true){
        auto logMsg2 = std::make_shared<modulog::communication::LogMessage>(modulog::communication::LogMessage::LOG_MSG_TYPE::LOG, "availableSpaceMB", std::to_string(spaceInfo.available/1024));
        agentClient.sendLog(logMsg2);
        auto logMsg3 = std::make_shared<modulog::communication::LogMessage>(modulog::communication::LogMessage::LOG_MSG_TYPE::LOG, "freeSpaceMB", std::to_string(spaceInfo.free/1024));
        agentClient.sendLog(logMsg3);
        std::this_thread::sleep_for(std::chrono::seconds(logInterval));
    }
    return 0;
}

