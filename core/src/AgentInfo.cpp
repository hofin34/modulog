//
// Created by martin on 09.12.21.
//

#include "AgentInfo.h"

#include <utility>

AgentInfo::AgentInfo(const std::string &agentId, const std::filesystem::path &agentPath, const std::shared_ptr<reproc::process>& agentProcess,
          const std::shared_ptr<reproc::options> &agentProcessOptions):
        agentId_(agentId), agentPath_(agentPath), agentProcess_(agentProcess), agentProcessOptions_(agentProcessOptions){

}
void AgentInfo::setAgentId(std::string id) {
    agentId_ = std::move(id);
}

std::string AgentInfo::getAgentId() {
    return agentId_;
}

std::shared_ptr<reproc::process> AgentInfo::getAgentProcess() {
    return agentProcess_;
}

void AgentInfo::stopAgent() {
    agentProcess_->stop(agentProcessOptions_->stop);
}

