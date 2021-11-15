//
// Created by martin on 15.11.21.
//

#include "Agent.h"

std::string Agent::getName() {
    return name_;
}

std::filesystem::path Agent::getPath() {
    return path_;
}

std::shared_ptr<reproc::process> Agent::getProcess() {
    return process_;
}

std::shared_ptr<reproc::options> Agent::getProcessOptions() {
    return processOptions_;
}