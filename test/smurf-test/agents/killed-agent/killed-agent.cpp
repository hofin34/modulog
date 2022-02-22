#include <modulog/agent_client/AgentClient.hpp>

int main(){
    auto ioContext = std::make_shared<asio::io_context>();
    modulog::agent_client::AgentClient agentClient(ioContext, false, "killed-agent");
    agentClient.initClient();
    for(int i = 0; i < 5; i++){ // just sending a few logs
        auto msg = std::make_shared<modulog::communication::LogMessage>(modulog::communication::LogMessage::LOG_MSG_TYPE::LOG, "simpleLog", "simpleValue" + std::to_string(i));
        agentClient.sendLog(msg);
    }
    std::this_thread::sleep_for(std::chrono::seconds(2));
    return EXIT_FAILURE; // simulates agent kill
}
