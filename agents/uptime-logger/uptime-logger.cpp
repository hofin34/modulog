#include "../../communication/include/AgentClient.h"
#include <asio.hpp>


int main(){
    auto ioContext = std::make_shared<asio::io_context>();
    AgentClient agentClient(ioContext, false, "ag-uptime-logger" );
    agentClient.initClient();
    auto programBegin = std::chrono::steady_clock::now();
    auto logStartMsg = std::make_shared<LogMessage>(LogMessage::LOG_MSG_TYPE::LOG, "agentStarted", "start");
    agentClient.sendLog(logStartMsg);
    while(true){
        auto now = std::chrono::steady_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::seconds>(now-programBegin).count();
        auto logMsg = std::make_shared<LogMessage>(LogMessage::LOG_MSG_TYPE::LOG, "howLongRunning", std::to_string(duration));
        std::cout << "Ag sending: "  << logMsg << std::endl;
        agentClient.sendLog(logMsg);

        std::this_thread::sleep_for(std::chrono::seconds(2));
    }

    return 0;
}
