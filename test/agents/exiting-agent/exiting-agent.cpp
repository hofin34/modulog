#include <modulog/agent_client/AgentClient.hpp>

int main(){
    auto ioContext = std::make_shared<asio::io_context>();
    modulog::agent_client::AgentClient agentClient(ioContext, false, "exiting-agent");
    agentClient.initClient();
    agentClient.exitConnection();
    return 0;
}
