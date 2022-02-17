#include <modulog/core/Agent.hpp>

namespace modulog::core{
    std::string Agent::getId() {
        return agentProcess_->getAgentId();
    }


    void Agent::setConfirmedAlive(bool value) {
        confirmedAlive_ = value;

    }

    bool Agent::getConfirmedAlive() {
        return confirmedAlive_;
    }


    void Agent::deleteSelf() {
        bringauto::logging::Logger::logInfo("Agent {} is stopping...", agentProcess_->getAgentId());
        messageExchanger_->getConnection()->closeConnection();
        agentProcess_->stopAgent();
    }

    std::shared_ptr<communication::MessageExchanger> Agent::getMessageExchanger() {
        return messageExchanger_;
    }




}
