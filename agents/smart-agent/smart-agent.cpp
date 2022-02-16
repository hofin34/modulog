/*
 * modulog agent, that monitors disk status (look at README.md for more info)
 */

#include <modulog/agent_client/AgentClient.hpp>
#include <modulog/agent_client/Helpers.hpp>

#include <thread>
#include <iostream>



int main(int argc, char** argv){
    auto programStart = std::chrono::system_clock::now();
    nlohmann::json configJson = modulog::agent_client::Helpers::parseConfig(argv [0]);

    if(!configJson.contains("id")){
        std::cerr << "Include config with id defined." << std::endl;
        throw std::runtime_error("...");
    }
    int logInterval = 4;
    if(configJson.contains("logInterval")){
        logInterval = configJson["logInterval"];
    }

    auto ioContext = std::make_shared<asio::io_context>();
    modulog::agent_client::AgentClient agentClient(ioContext, false, configJson["id"] );
    agentClient.initClient();
    while(true){

        auto logMsg = std::make_shared<modulog::communication::LogMessage>(modulog::communication::LogMessage::LOG_MSG_TYPE::LOG, "notImplemented", "agent not implemented");
        agentClient.sendLog(logMsg);

        std::this_thread::sleep_for(std::chrono::seconds(logInterval));
    }
    return 0;
}

