#include <iostream>
#include "subprocess.h"
#include <csignal>
#include <reproc++/drain.hpp>
#include <reproc++/reproc.hpp>
#include <vector>

struct AgentInfo{
    std::string name;
    int terminate_timeout = 2000;
    reproc::process process;
};
std::vector<std::shared_ptr<reproc::process>> running_processes;

void cleanup(int signum){
    std::cout << "cleanup..." << std::endl;
    reproc::options options;
    options.stop = {
            { reproc::stop::terminate, reproc::milliseconds(2000) },
            { reproc::stop::kill, reproc::milliseconds(2000) }
    };
    for(auto &proc : running_processes){
        std::cout << "Stopping process " << proc->pid().first << std::endl;
        proc->stop(options.stop);
    }

    exit(0);
}

int main(int argc, const char **argv) {
    signal(SIGINT, cleanup);
    signal(SIGTERM, cleanup);

    reproc::options options;
    options.redirect.parent = true;
    auto process = std::make_shared<reproc::process>();
    std::vector<std::string> v;
    v.emplace_back("./process1");
    std::error_code ec = process->start(v, options);
    std::cout << "HERE" << std::endl;
    if (ec == std::errc::no_such_file_or_directory) {
        std::cerr << "Program not found. Make sure it's available from the PATH.";
        return ec.value();
    } else if (ec) {
        std::cout << "HERsE" << std::endl;
        return ec.value();
    }
    running_processes.push_back(process);
    std::cout << "HEREew" << std::endl;
    int pid = 0;
    std::tie(pid, ec) = process->pid();
    std::cout << pid << std::endl;
    unsigned int microsecond = 1000000;
    usleep(1 * microsecond);//sleeps for 3 second


    while(true);

    return 0;

}
