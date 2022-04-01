#include <modulog/agent_client/AgentClient.hpp>
#include <modulog/agent_client/ClientFactory.hpp>

int main(){
    auto ioContext = std::make_shared<asio::io_context>();
    auto agentClient = modulog::agent_client::ClientFactory::createClient(ioContext, "permanent-log-agent");
    agentClient->initClient();
    while(agentClient->canLog()){
        auto msg = std::make_shared<modulog::communication::LogMessage>(modulog::communication::LogMessage::LOG_MSG_TYPE::LOG, "simpleLog", "simpleValue");
        agentClient->sendLog(msg);
        std::this_thread::sleep_for(std::chrono::milliseconds(50));
    }
    agentClient->exitConnection();
}
