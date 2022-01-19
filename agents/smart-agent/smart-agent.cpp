#include <iostream>
#include <asio.hpp>
#include "../../communication/include/MessageSerializer.hpp"
#include "../../communication/include/AgentClient.hpp"
#include "Helpers.hpp"
#include <thread>

// temp:sudo smartctl -A /dev/nvme0n1 | grep -i temperature:


int main(int argc, char** argv){
    auto programStart = std::chrono::system_clock::now();
    nlohmann::json configJson = Helpers::parseConfig(argv[0]);

    if(!configJson.contains("id")){
        std::cerr << "Include config with id defined." << std::endl;
        throw std::runtime_error("...");
    }
    int logInterval = 4;
    if(configJson.contains("logInterval")){
        logInterval = configJson["logInterval"];
    }

    auto ioContext = std::make_shared<asio::io_context>();
    AgentClient agentClient(ioContext, false, configJson["id"] );
    agentClient.initClient();
    while(true){

        auto logMsg = std::make_shared<LogMessage>(LogMessage::LOG_MSG_TYPE::LOG, "temperature", "agent not implemented");
        agentClient.sendLog(logMsg);

        std::this_thread::sleep_for(std::chrono::seconds(logInterval));
    }
    return 0;
}

