//
// Created by martin on 15.11.21.
//

#include <iostream>
#include <csignal>
#include <cstring>
#include <unistd.h>
#include "AgentHandler.h"

void AgentHandler::cleanup(int sigNum) {
    std::cout << "cleanup..." << std::endl; // TODO
    /*
    for(auto &agent : running_agents){
        std::cout << "Stopping " << agent.getName() << " PID: " << agent.getProcess()->pid().first << std::endl;
        std::error_code ec;
        int status;
        std::tie(status, ec) = agent.getProcess()->stop(agent.getProcessOptions()->stop);

    }
     */
    exit(0);
}

int AgentHandler::createAgents() {
    struct sigaction act{};
    memset(&act, 0, sizeof(act));
    act.sa_handler = &AgentHandler::cleanup;
    sigaction(SIGINT,  &act, nullptr);
    sigaction(SIGTERM, &act, nullptr);

    // Agent definition:
    std::string ag1Name = "Agent 1";
    std::string ag1Path = "./process1";
    int ag1TerminateTimeout = 2000;
    // ---
    std::cout << "Core PID: " << getpid() << std::endl;
    reproc::options options;
    options.redirect.parent = true;
    auto process = std::make_shared<reproc::process>();
    std::vector<std::string> v;
    v.emplace_back(ag1Path);
    std::error_code ec = process->start(v, options);
    if (ec == std::errc::no_such_file_or_directory) {
        std::cerr << "Program not found. Make sure it's available from the PATH.";
        return ec.value();
    } else if (ec) {
        std::cout << "HERE..." << std::endl;
        return ec.value();
    }
    Agent agent(ag1Name, ag1TerminateTimeout, process);
    running_agents.push_back(std::move(agent));
    unsigned int microsecond = 1000000;
    usleep(1 * microsecond);//sleeps for 3 second
    while(true);
}
