#include <modulog/agent_client/AgentClient.hpp>
#include <modulog/agent_client/ClientFactory.hpp>

int main() {
    auto ioContext = std::make_shared<asio::io_context>();
    auto agentClient = modulog::agent_client::ClientFactory::createClient(ioContext, "big-log-agent");
    agentClient->initClient();
    std::string bigString = "";
    for (int i = 0; i < 20000; i++){
        bigString += std::to_string(i);
    }
    auto msg = std::make_shared<modulog::communication::LogMessage>(
            modulog::communication::LogMessage::LOG_MSG_TYPE::LOG, "simpleLog", bigString);
    agentClient->sendLog(msg);
    agentClient->exitConnection();
}
