//
// Created by martin on 15.11.21.
//

#include "../include/Agent.h"
#include "../../communication/include/MessageDeserializer.h"

std::string Agent::getId() {
    return id_;
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

void Agent::deleteSelf() {
    std::cout << "Agent " << id_ << " is deleting..." << std::endl;
    //TODO some cleaning
}

std::shared_ptr<MessageExchanger> Agent::getMessageExchanger() {
    return messageExchanger_;
}


