#include "../../agent-client/include/modulog/agent-client/AgentClient.hpp"
#include "modulog/agent_client/Helpers.hpp"
#include <asio.hpp>
#include <random>


int main(int argc, char** argv){
    nlohmann::json configJson = Helpers::parseConfig(argv[0]);
    if(!configJson.contains("id")){
        std::cerr << "Include config with id defined." << std::endl;
        exit(EXIT_FAILURE);
    }
    int logIntervalMin;
    if(configJson.contains("logIntervalMin")){
        logIntervalMin = configJson["logIntervalMin"];
    }else{
        std::cerr << "specify logIntervalMin" << std::endl;
        exit(EXIT_FAILURE);
    }
    int logIntervalMax;
    if(configJson.contains("logIntervalMax")){
        logIntervalMax = configJson["logIntervalMax"];
    }else{
        std::cerr << "specify logIntervalMax" << std::endl;
        exit(EXIT_FAILURE);
    }

    auto ioContext = std::make_shared<asio::io_context>();
    AgentClient agentClient(ioContext, false, configJson["id"]);
    agentClient.initClient();

    std::random_device rd;
    std::default_random_engine eng(rd());
    std::uniform_int_distribution<int> distr(logIntervalMin, logIntervalMax);
    while(true){
        int newRandom = distr(eng);
        std::cout << newRandom << std::endl;
        auto logMsg = std::make_shared<LogMessage>(LogMessage::LOG_MSG_TYPE::LOG, "randomTime", std::to_string(newRandom));
        agentClient.sendLog(logMsg);
        std::this_thread::sleep_for(std::chrono::milliseconds (newRandom));
    }

    return 0;
}
