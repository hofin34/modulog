#include <modulog/agent_client/AgentClient.hpp>

class FreezedClient : public modulog::agent_client::AgentClient{
public:
    using modulog::agent_client::AgentClient::AgentClient;
    void handleResponses(){
        std::cout << "freezed response handling overriden..." << std::endl;
        while(true){
            std::this_thread::sleep_for(std::chrono::seconds(1));
        }
    }
};

int main(){
    std::cout <<"here" <<std::endl;
    auto ioContext = std::make_shared<asio::io_context>();
    FreezedClient freezedClient(ioContext, "freezed-agent");
    freezedClient.initClient();
    for(int i = 0; i < 2; i++){ // just sending a few logs
        auto msg = std::make_shared<modulog::communication::LogMessage>(modulog::communication::LogMessage::LOG_MSG_TYPE::LOG, "simpleLog", "simpleValue" + std::to_string(i));
        freezedClient.sendLog(msg);
    }
    while (true)
        std::this_thread::sleep_for(std::chrono::seconds(1));
}
