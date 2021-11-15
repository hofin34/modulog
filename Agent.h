#pragma once


#include <string>
#include <filesystem>
#include <reproc++/reproc.hpp>

class Agent {
public:
    Agent(std::string name, int terminate_timeout, std::shared_ptr<reproc::process>& process):
            name_(std::move(name)),
            process_(process) {
        processOptions_->stop = {
                { reproc::stop::terminate, reproc::milliseconds(terminate_timeout) },
                { reproc::stop::kill, reproc::milliseconds(0) }
        };
    }
    // -------
    std::string getName();
    std::filesystem::path getPath();
    std::shared_ptr<reproc::process> getProcess();
    std::shared_ptr<reproc::options> getProcessOptions();

private:
    std::string name_;
    std::filesystem::path path_;
    std::shared_ptr<reproc::process> process_;
    std::shared_ptr<reproc::options> processOptions_;
};