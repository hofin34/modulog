#include <modulog/agent_client/AgentClient.hpp>
#include <modulog/agent_client/TcpAgentClient.hpp>
#include <thread>
#include <asio.hpp>

class FreezedClient : public modulog::agent_client::TcpAgentClient{
public:
    using modulog::agent_client::TcpAgentClient::TcpAgentClient;
    void handleResponses(){
        std::cout << "freezed response handling overriden..." << std::endl;
        while(true){
            std::this_thread::sleep_for(std::chrono::seconds(1));
        }
    }
};

int main(){
    auto ioContext = std::make_shared<asio::io_context>();
    std::shared_ptr<modulog::agent_client::AgentClient> freezedClient = std::make_shared<FreezedClient>(ioContext, "freezed-agent");
    freezedClient->initClient();
    for(int i = 0; i < 2; i++){ // just sending a few logs
        auto msg = std::make_shared<modulog::communication::LogMessage>(modulog::communication::LogMessage::LOG_MSG_TYPE::LOG, "simpleLog", "simpleValue" + std::to_string(i));
        freezedClient->sendLog(msg);
    }
    while (true)
        std::this_thread::sleep_for(std::chrono::seconds(1));
}
