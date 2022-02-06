#include <modulog/communication/MessageSerializer.hpp>
#include <modulog/agent_client/AgentClient.hpp>
#include <modulog/agent_client/Helpers.hpp>

#include <thread>
#include <sys/sysinfo.h>
#include <iostream>
#include <asio.hpp>

long long getRAM(){
    struct sysinfo memInfo{};
    sysinfo (&memInfo);
    long long freeRam = memInfo.freeram;
    freeRam *= memInfo.mem_unit;
    freeRam /= (1024 * 1024); //to get MiB
    return freeRam;
}

int main(int argc, char** argv){
    auto programStart = std::chrono::system_clock::now();
    nlohmann::json configJson = modulog::agent_client::Helpers::parseConfig(argv[0]);// nlohmann::json::parse(ifs);

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
    modulog::agent_client::AgentClient agentClient(ioContext, false, configJson["id"] );
    agentClient.initClient();
    while(true){
        auto freeRam = getRAM();
        if(freeNotSmallerThan > freeRam){
            auto errMsg = std::make_shared<modulog::communication::LogMessage>(modulog::communication::LogMessage::LOG_MSG_TYPE::ERROR, "freeRamMiB", std::to_string(freeRam));
            agentClient.sendLog(errMsg);
        }else{
            auto logMsg = std::make_shared<modulog::communication::LogMessage>(modulog::communication::LogMessage::LOG_MSG_TYPE::LOG, "freeRamMiB", std::to_string(freeRam));
            agentClient.sendLog(logMsg);
        }

        std::this_thread::sleep_for(std::chrono::seconds(logInterval));
    }
    return 0;
}

