#include <modulog/core/AgentProcess.hpp>

namespace modulog::core{
//TODO if in agents list not existing agent - tell and end with cleaning
    AgentProcess::AgentProcess(const std::string &agentId, const std::filesystem::path &agentPath, const std::shared_ptr<reproc::process>& agentProcess,
                               const std::shared_ptr<reproc::options> &agentProcessOptions):
            agentId_(agentId), agentPath_(agentPath), agentProcess_(agentProcess), agentProcessOptions_(agentProcessOptions){

    }
    void AgentProcess::setAgentId(std::string id) {
        agentId_ = std::move(id);
    }

    std::string AgentProcess::getAgentId() {
        return agentId_;
    }


    void AgentProcess::stopAgent() {
        agentProcess_->stop(agentProcessOptions_->stop);
    }


}

