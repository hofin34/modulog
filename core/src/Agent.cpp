//
// Created by martin on 15.11.21.
//

#include "../include/Agent.h"
#include "../../communication/include/MessageDeserializer.h"

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




void Agent::setConfirmedAlive(bool value) {
    confirmedAlive_ = value;

}

bool Agent::getConfirmedAlive() {
    return confirmedAlive_;
}

void Agent::setId(const std::string& id) {
    id_ = id;
}

TcpConnection::pointer Agent::getTcpConnection() {
    return tcpConnection_;
}


