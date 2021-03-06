/*
 * modulog agent, that monitors, how long is app running (look at README.md for more info)
 */

#include <modulog/agent_client/AgentClient.hpp>
#include <modulog/agent_client/Helpers.hpp>
#include <modulog/agent_client/ClientFactory.hpp>


int main(int argc, char **argv) {
    nlohmann::json configJson = modulog::agent_client::Helpers::parseConfig(argv[0]);
    if (!configJson.contains("id")) {
        std::cerr << "Include config with id defined." << std::endl;
        return EXIT_FAILURE;
    }
    int logInterval = 4;
    if (configJson.contains("logInterval")) {
        logInterval = configJson["logInterval"];
    }
    auto ioContext = std::make_shared<asio::io_context>();
    auto agentClient = modulog::agent_client::ClientFactory::createClient(ioContext, configJson["id"]);
    agentClient->initClient();
    auto programBegin = std::chrono::steady_clock::now();
    auto logStartMsg = std::make_shared<modulog::communication::LogMessage>(
            modulog::communication::LogMessage::LOG_MSG_TYPE::LOG, "agentStarted", "start");
    agentClient->sendLog(logStartMsg);
    while (agentClient->canLog()) {
        auto now = std::chrono::steady_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::seconds>(now - programBegin).count();
        auto logMsg = std::make_shared<modulog::communication::LogMessage>(
                modulog::communication::LogMessage::LOG_MSG_TYPE::LOG, "howLongRunning", std::to_string(duration));
        agentClient->sendLog(logMsg);

        agentClient->sleepFor(std::chrono::seconds(logInterval));
    }
    return 0;
}
