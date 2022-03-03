#pragma once
#include <modulog/meta_lib/StateGraphHandler.hpp>
#ifdef BRINGAUTO_TESTS
    #include <state_smurf/diagram/StateDiagram.hpp>
    #include <state_smurf/transition/StateTransition.hpp>
#endif

#include <filesystem>

namespace modulog::meta_lib{
    struct SharedSettings{
        struct ServerSettings{
            int portNumber = 1234; // Port number, where will be TCP server listening
            int connectionTimeoutSec = 3; // How many seconds has agent to connect to server before is killed
            const int MAX_PACKET_SIZE = 512; // Packet size, which will be read in TcpConnection
        } ServerSettings;
        struct LogSettings{
            std::filesystem::path enabledAgentsPath = "./agents-enabled.conf";
            std::filesystem::path logsDestination = "./logs"; // Where will be saved logs (folder is created if not existing)
            int isAliveIntervalSec = 8; // How often check, if agent is not freezed (in seconds)
            int isAliveTimeoutSec = 2; // How long wait for agents isAlive response
            std::filesystem::path sharedAgentsConfig = ""; // Path to file - its content will be sent to all agents (if not using, leave this variable empty)
        } LogSettings;

        struct Testing{
            #ifdef BRINGAUTO_TESTS
                std::shared_ptr<state_smurf::transition::StateTransition> transitions = std::make_shared<state_smurf::transition::StateTransition>(StateGraphHandler::createStateDiagram());
            #endif
        } Testing;
    };
}
