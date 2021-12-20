//
// Created by martin on 15.11.21.
//

#include "../include/Agent.h"
#include "../../communication/include/MessageDeserializer.h"

std::string Agent::getId() {
    return agentInfo_->getAgentId();
}


void Agent::setConfirmedAlive(bool value) {
    confirmedAlive_ = value;

}

bool Agent::getConfirmedAlive() {
    return confirmedAlive_;
}


void Agent::deleteSelf() {
    std::cout << "Agent " << id_ << " is deleting..." << std::endl;
    messageExchanger_->getConnection()->close_connection();
    agentInfo_->stopAgent();
}

std::shared_ptr<MessageExchanger> Agent::getMessageExchanger() {
    return messageExchanger_;
}


