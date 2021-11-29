#include "../../communication/include/AgentClient.h"
#include <asio.hpp>


int main(){
    auto ioContext = std::make_shared<asio::io_context>();
    AgentClient agentClient(ioContext, false, "ag-uptime-logger" );
    while(true){
        auto logMsg = std::make_shared<LogMessage>(LogMessage::LOG_MSG_TYPE::LOG, "lastUpChecks", "asdf");
        std::cout << "Ag sending: "  << std::endl;
        agentClient.sendLog(logMsg);
        std::this_thread::sleep_for(std::chrono::seconds(2));
    }

    return 0;
}
