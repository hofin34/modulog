#include <modulog/agent_client/AgentClient.hpp>
#include <modulog/agent_client/ClientFactory.hpp>

int main(){
    auto ioContext = std::make_shared<asio::io_context>();
    auto agentClient = modulog::agent_client::ClientFactory::createClient(ioContext, "on-off-agent");
    agentClient->initClient();
    agentClient->exitConnection();
    return 0;
}
