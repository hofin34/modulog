#include "modulog/agent_client/AgentClient.hpp"

int main(int argc, char** argv){
    while (true)
        std::this_thread::sleep_for(std::chrono::seconds(10));
    return 0;
}
