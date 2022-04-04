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
        } ServerSettings;
        struct LogSettings{
            std::filesystem::path enabledAgentsPath = "./agents-enabled.conf";
            std::filesystem::path logsDestination = "./logs/"; // Where will be saved logs (folder must exists)
            int isAliveIntervalSec = 8; // How often check, if agent is not freezed (in seconds)
            int isAliveTimeoutSec = 2; // How long wait for agents isAlive response
            int exitSendTimeoutSec = 2; // How many seconds will agent have to send remaining logs, when is whole program interrupted
            std::filesystem::path sharedAgentsConfig = ""; // Path to file - its content will be sent to all agents (if not using, leave this variable empty)
            bool oneFileLog = false; // if true, all logs will be saved into one file (you can change this value through cmd switch)
            int maxLogFileSize = 5 * 1024 * 1024; // what maximum size can have one log file in bytes
        } LogSettings;

        #ifdef BRINGAUTO_TESTS
        struct Testing{
                std::shared_ptr<state_smurf::transition::StateTransition> transitions; // this must be initialized by function initTesting()!
                void initTesting(){
                    transitions = std::make_shared<state_smurf::transition::StateTransition>(StateGraphHandler::createStateDiagram());
                };
        } Testing;
        #endif
    };
}
