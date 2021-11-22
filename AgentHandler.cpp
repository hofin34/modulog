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
    //TODO
    return 0;
}

std::shared_ptr<Agent> AgentHandler::createNextAgent() {
    struct sigaction act{};
    memset(&act, 0, sizeof(act));
    act.sa_handler = &AgentHandler::cleanup;
    sigaction(SIGINT,  &act, nullptr);
    sigaction(SIGTERM, &act, nullptr);

    // Agent definition:
    std::string ag1Id = "Agent-1";
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
        throw std::runtime_error("Program not found. Make sure it's available from the PATH");
    } else if (ec) {
        throw std::runtime_error("Agent creation err:" + ec.message());
    }
    auto agent = std::make_shared<Agent>(ag1Id, ag1TerminateTimeout, process);
    std::cout << "Created agent with PID: " << agent->getProcessPid() << std::endl;
    running_agents.push_back(agent);
    return agent;
}
