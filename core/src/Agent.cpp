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

void Agent::processMessages(int maxCount) {
    for(int i = 0; i<maxCount; i++){
        auto msgString = tcpConnection_->popMessage();
        if(msgString == nullptr)
            return;
        MessageDeserializer messageDeserializer(*msgString);
        if(messageDeserializer.getMsgType() == Message::MSG_TYPE::CONTROL_MSG){
            controlMessages_.push_back(messageDeserializer.getControlMessage());
        }else if(messageDeserializer.getMsgType() == Message::MSG_TYPE::LOG_MSG){
            logMessages_.push_back(messageDeserializer.getLogMessage());
        }else{
            std::cerr << "Bad message type!" << std::endl;
        }
    }
}

std::shared_ptr<LogMessage> Agent::popLogMessage() {
    processMessages(5); // TODO how many???
    if(logMessages_.empty())
        return nullptr;
    auto toReturn = logMessages_.front();
    logMessages_.erase(logMessages_.begin());
    return toReturn;
}

std::shared_ptr<ControlMessage> Agent::popControlMessage() {
    processMessages(5); // TODO how many???
    if(controlMessages_.empty())
        return nullptr;
    auto toReturn = controlMessages_.front();
    controlMessages_.erase(controlMessages_.begin());
    return toReturn;
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

