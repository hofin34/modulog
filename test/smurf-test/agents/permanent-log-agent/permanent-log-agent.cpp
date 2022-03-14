#include <modulog/agent_client/AgentClient.hpp>

int main(){
    auto ioContext = std::make_shared<asio::io_context>();
    modulog::agent_client::AgentClient agentClient(ioContext, "simple-log-agent");
    agentClient.initClient();
    while(true){
        auto msg = std::make_shared<modulog::communication::LogMessage>(modulog::communication::LogMessage::LOG_MSG_TYPE::LOG, "simpleLog", "simpleValue");
        agentClient.sendLog(msg);
        std::this_thread::sleep_for(std::chrono::milliseconds(50));
    }
    agentClient.exitConnection();
}
