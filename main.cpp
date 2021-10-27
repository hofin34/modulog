#include <iostream>
#include "subprocess.h"
#include <csignal>
#include <reproc++/drain.hpp>
#include <reproc++/reproc.hpp>
#include <utility>
#include <vector>
#include <cstring>
#include <filesystem>

struct Agent{
    Agent(std::string name, int terminate_timeout, std::shared_ptr<reproc::process>& process):
    name(std::move(name)),
    process(process) {
        process_options.stop = {
                { reproc::stop::terminate, reproc::milliseconds(terminate_timeout) },
                { reproc::stop::kill, reproc::milliseconds(0) }
        };
    }
    // -------
    std::string name;
    std::filesystem::path path;
    std::shared_ptr<reproc::process> process;
    reproc::options process_options;
};
std::vector<Agent> running_agents;

void cleanup(int signum){
    std::cout << "cleanup..." << std::endl;
    for(auto &agent : running_agents){
        std::cout << "Stopping " << agent.name << " PID: " << agent.process->pid().first << std::endl;
        std::error_code ec;
        int status;
        std::tie(status, ec) = agent.process->stop(agent.process_options.stop);

    }
    exit(0);
}

int main(int argc, const char **argv) {
    struct sigaction act{};
    memset(&act, 0, sizeof(act));
    act.sa_handler = cleanup;
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
