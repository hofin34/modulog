#include <modulog/agent_client/AgentClient.hpp>
#include <modulog/agent_client/Helpers.hpp>



int main(int argc, char** argv){
    nlohmann::json configJson = modulog::agent_client::Helpers::parseConfig(argv[0]);
    if(!configJson.contains("id")){
        std::cerr << "Include config with id defined." << std::endl;
        throw std::runtime_error("...");
    }
    int logInterval = 4;
    if(configJson.contains("logInterval")){
        logInterval = configJson["logInterval"];
    }

    auto ioContext = std::make_shared<asio::io_context>();
    modulog::agent_client::AgentClient agentClient(ioContext, false, configJson["id"]);
    agentClient.initClient();
    auto programBegin = std::chrono::steady_clock::now();
    auto logStartMsg = std::make_shared<modulog::communication::LogMessage>(modulog::communication::LogMessage::LOG_MSG_TYPE::LOG, "agentStarted", "start");
    agentClient.sendLog(logStartMsg);
    while(true){
        auto now = std::chrono::steady_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::seconds>(now-programBegin).count();
        auto logMsg = std::make_shared<modulog::communication::LogMessage>(modulog::communication::LogMessage::LOG_MSG_TYPE::LOG, "howLongRunning", std::to_string(duration));
        agentClient.sendLog(logMsg);

        std::this_thread::sleep_for(std::chrono::seconds(logInterval));
        //agentClient.exitConnection();
        //exit(4);
    }

    return 0;
}
