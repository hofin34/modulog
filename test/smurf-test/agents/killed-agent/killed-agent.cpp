#include "modulog/agent_client/AgentClient.hpp"

int main(){
    auto ioContext = std::make_shared<asio::io_context>();
    modulog::agent_client::AgentClient agentClient(ioContext, false, "killed-agent");
    agentClient.initClient();
    for(int i = 0; i < 1000; i++){ // just sending a few logs
        auto msg = std::make_shared<modulog::communication::LogMessage>(modulog::communication::LogMessage::LOG_MSG_TYPE::LOG, "simpleLog", "simpleValue" + std::to_string(i));
        agentClient.sendLog(msg);
    }
    return EXIT_FAILURE; // simulates agent kill
}
