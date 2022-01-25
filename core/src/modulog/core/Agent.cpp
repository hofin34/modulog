#include <modulog/core/Agent.hpp>

namespace modulog::core{
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
        std::cout << "Agent " << agentInfo_->getAgentId() << " is deleting..." << std::endl;
        agentInfo_->stopAgent();
    }

    std::shared_ptr<communication::MessageExchanger> Agent::getMessageExchanger() {
        return messageExchanger_;
    }



}
