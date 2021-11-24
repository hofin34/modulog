//
// Created by martin on 15.11.21.
//

#include <iostream>
#include <csignal>
#include <cstring>
#include <unistd.h>
#include "AgentHandler.h"

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
    if(createdAgents>=agentsJsonConfigs_.size())
        return nullptr;
    nlohmann::json actConfig = agentsJsonConfigs_.at(createdAgents);
    if(!actConfig.contains("id"))
        throw std::runtime_error("Json config not containing id");
    if(!actConfig.contains("execPath"))
        throw std::runtime_error("Json config not containing execPath");

    // Agent definition:
    std::string agentId = actConfig["id"];
    std::string agentPath = actConfig["execPath"];
    int agentTerminateTimeout;
    if(actConfig.contains("terminateTimeout")){
        agentTerminateTimeout = actConfig["terminateTimeout"];
    }else {
        agentTerminateTimeout = 2000;
    }
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
    auto agent = std::make_shared<Agent>(agentId, agentTerminateTimeout, process, actConfig);
    std::cout << "Created agent with PID: " << agent->getProcessPid() << std::endl;
    runningAgents_.push_back(agent);
    return agent;
}

const std::vector<std::shared_ptr<Agent>> &AgentHandler::getRunningAgents() {
    return runningAgents_;
}

void AgentHandler::loadAgentsConfigs(const std::filesystem::path& pathToDir) {
    std::string path = pathToDir;
    try{
        for (const auto & entry : std::filesystem::directory_iterator(path)){
            try{
                std::ifstream ifs(entry.path());
                nlohmann::json jsonConf = nlohmann::json::parse((ifs));
                agentsJsonConfigs_.push_back(jsonConf);
            }catch(...){
                std::cerr << "Couldn't parse json config (or file open err)." << std::endl;
            }
        }
        std::cout << "parsed" << std::endl;
    }catch(std::exception& e){
        throw std::runtime_error("Failed parsing agents configs: " + std::string(e.what()));
    }

}
