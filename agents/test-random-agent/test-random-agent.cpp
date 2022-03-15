/*
 * modulog agent, that send logs at random intervals - for testing purposes (look at README.md for more info)
 */

#include <modulog/agent_client/Helpers.hpp>
#include <modulog/agent_client/AgentClient.hpp>

#include <random>


int main(int argc, char** argv){
    nlohmann::json configJson = modulog::agent_client::Helpers::parseConfig(argv[0]);
    if(!configJson.contains("id")){
        std::cerr << "Include config with id defined." << std::endl;
        return EXIT_FAILURE;
    }
    int logIntervalMin;
    if(configJson.contains("logIntervalMin")){
        logIntervalMin = configJson["logIntervalMin"];
    }else{
        std::cerr << "specify logIntervalMin" << std::endl;
        return EXIT_FAILURE;
    }
    int logIntervalMax;
    if(configJson.contains("logIntervalMax")){
        logIntervalMax = configJson["logIntervalMax"];
    }else{
        std::cerr << "specify logIntervalMax" << std::endl;
        return EXIT_FAILURE;
    }

    auto ioContext = std::make_shared<asio::io_context>();
    modulog::agent_client::AgentClient agentClient(ioContext, configJson["id"]);
    agentClient.initClient();

    std::random_device rd;
    std::default_random_engine eng(rd());
    std::uniform_int_distribution<int> distr(logIntervalMin, logIntervalMax);
    while(true){
        int newRandom = distr(eng);
        std::cout << newRandom << std::endl;
        auto logMsg = std::make_shared<modulog::communication::LogMessage>(modulog::communication::LogMessage::LOG_MSG_TYPE::LOG, "randomTime", std::to_string(newRandom));
        agentClient.sendLog(logMsg);
        std::this_thread::sleep_for(std::chrono::milliseconds (newRandom));
    }

    return 0;
}
