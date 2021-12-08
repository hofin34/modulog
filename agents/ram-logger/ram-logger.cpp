#include <iostream>
#include <asio.hpp>
#include "../../communication/include/MessageSerializer.h"
#include "../../communication/include/AgentClient.h"
#include <thread>
#include <sys/statvfs.h>
#include <filesystem>
#include <fstream>
#include "sys/types.h"
#include "sys/sysinfo.h"

long long getFreeRam(){
    struct sysinfo memInfo;
    sysinfo (&memInfo);
    long long physMemUsed = memInfo.freeram;
    physMemUsed *= memInfo.mem_unit;
    physMemUsed /= (1024*1024); //to get MiB
    return physMemUsed;
}

int main(int argc, char** argv){
    nlohmann::json configJson = AgentClient::parseConfig(argv[0]);// nlohmann::json::parse(ifs);

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
        auto freeRam = getFreeRam();
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

