#include <modulog/agent_client/AgentClient.hpp>

int main(){
    auto ioContext = std::make_shared<asio::io_context>();
    modulog::agent_client::AgentClient agentClient(ioContext, false, "simple-log-agent");
    agentClient.initClient();
    auto msg = std::make_shared<modulog::communication::LogMessage>(modulog::communication::LogMessage::LOG_MSG_TYPE::LOG, "simpleLog", "simpleValue");
    while (true){
        std::this_thread::sleep_for(std::chrono::milliseconds(800));
        agentClient.sendLog(msg);
    }
    //agentClient.exitConnection();
}
