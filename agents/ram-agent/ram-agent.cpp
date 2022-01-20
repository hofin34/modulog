#include <iostream>
#include <asio.hpp>
#include "modulog/communication/MessageSerializer.hpp"
#include "../../agent-client/include/modulog/agent-client/AgentClient.hpp"
#include <thread>
#include "sys/sysinfo.h"
#include "modulog/agent_client/Helpers.hpp"

long long getTemperature(){
    struct sysinfo memInfo;
    sysinfo (&memInfo);
    long long physMemUsed = memInfo.freeram;
    physMemUsed *= memInfo.mem_unit;
    physMemUsed /= (1024*1024); //to get MiB
    return physMemUsed;
}

int main(int argc, char** argv){
    auto programStart = std::chrono::system_clock::now();
    nlohmann::json configJson = Helpers::parseConfig(argv[0]);// nlohmann::json::parse(ifs);

    if(!configJson.contains("id")){
        std::cerr << "Include config with id defined." << std::endl;
        throw std::runtime_error("...");
    }
    int freeNotSmallerThan = 0;
    if(configJson.contains("freeNotSmallerThan")){
        freeNotSmallerThan = configJson["freeNotSmallerThan"];
    }
    int logInterval = 4; // TODO default in variable
    if(configJson.contains("logInterval")){
        logInterval = configJson["logInterval"];
    }


    auto ioContext = std::make_shared<asio::io_context>();
    AgentClient agentClient(ioContext, false, configJson["id"] );
    agentClient.initClient();
    while(true){
        auto freeRam = getTemperature();
        if(freeNotSmallerThan > freeRam){
            auto errMsg = std::make_shared<LogMessage>(LogMessage::LOG_MSG_TYPE::ERROR, "freeRamMiB", std::to_string(freeRam));
            agentClient.sendLog(errMsg);
        }else{
            auto logMsg = std::make_shared<LogMessage>(LogMessage::LOG_MSG_TYPE::LOG, "freeRamMiB", std::to_string(freeRam));
            agentClient.sendLog(logMsg);
        }

        std::this_thread::sleep_for(std::chrono::seconds(logInterval));
    }
    return 0;
}

