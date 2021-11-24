//
// Created by martin on 15.11.21.
//

#include "Agent.h"

std::string Agent::getId() {
    return id_;
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

void Agent::setConnection(const TcpConnection::pointer& connection) {
    tcpConnection_ = connection;
}

TcpConnection::pointer Agent::getConnection() {
    return tcpConnection_;
}

int Agent::getProcessPid() {
    return process_->pid().first;
}

void Agent::setId(const std::string& id) {
    id_ = id;
}
