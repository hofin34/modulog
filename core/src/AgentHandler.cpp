//
// Created by martin on 15.11.21.
//

#include <iostream>
#include <csignal>
#include <cstring>
#include <unistd.h>
#include "../include/AgentHandler.h"

AgentHandler::AgentHandler(const std::filesystem::path& pathToAgentsConfigDir): runningAgents_(){
    //TODO termination
    struct sigaction act{};
    memset(&act, 0, sizeof(act));
    act.sa_handler = &AgentHandler::cleanup;
    sigaction(SIGINT,  &act, nullptr);
    sigaction(SIGTERM, &act, nullptr);

    loadAgentsConfigs(pathToAgentsConfigDir);
};

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



std::shared_ptr<Agent> AgentHandler::createNextAgent() {
    if(createdAgents>=agentsPaths_.size())
        return nullptr;
    std::filesystem::path actAgentFolder = std::filesystem::absolute(agentsPaths_.at(createdAgents));
    std::string folderName = actAgentFolder.filename();
    std::filesystem::path execPath = actAgentFolder/folderName;
    if(!std::filesystem::exists(execPath)){
        std::cerr << "Executable " << execPath << " not found" << std::endl;
        throw std::runtime_error("Executable " + std::string(execPath) + " not found");
    }
    // Agent definition:
    std::string agentId = actAgentFolder; // is replaced after communication with client with real id from client side
    std::string agentPath = execPath;
    // ---
    reproc::options options;
    options.redirect.parent = true;
    auto process = std::make_shared<reproc::process>();
    std::vector<std::string> v;
    v.emplace_back(agentPath);
    std::error_code ec = process->start(v, options);
    createdAgents++;
    if (ec == std::errc::no_such_file_or_directory) {
        throw std::runtime_error("Program not found. Make sure it's available from the PATH");
    } else if (ec) {
        throw std::runtime_error("Agent creation err:" + ec.message());
    }
    auto agent = std::make_shared<Agent>(agentId, process);
    std::cout << "Created agent with PID: " << agent->getProcessPid() << std::endl;
    runningAgents_.push_back(agent);
    return agent;

}

const std::vector<std::shared_ptr<Agent>> &AgentHandler::getRunningAgents() {
    return runningAgents_;
}

void AgentHandler::loadAgentsConfigs(const std::filesystem::path& pathToListFile) {
    try{
        std::ifstream input( pathToListFile);
        for( std::string line; getline( input, line ); )
        {
            //TODO if empty lines in config etc.
            std::filesystem::path actAgentPath = line;
            agentsPaths_.push_back(actAgentPath);
        }
    }catch(...){
        std::cerr << "Couldn't parse json config (or file open err)." << std::endl;
    }

}
