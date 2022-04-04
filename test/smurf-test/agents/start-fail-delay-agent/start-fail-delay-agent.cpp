#include <thread>
#include "modulog/agent_client/AgentClient.hpp"

int main(int argc, char** argv){
    std::this_thread::sleep_for(std::chrono::milliseconds(800));
    return EXIT_FAILURE;
}
