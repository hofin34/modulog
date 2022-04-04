#include <modulog/core/AgentProcess.hpp>
#include <utility>

namespace modulog::core{
    AgentProcess::AgentProcess(std::string agentId, std::filesystem::path agentPath, std::shared_ptr<reproc::process>  agentProcess,
                               std::shared_ptr<reproc::options> agentProcessOptions):
            agentId_(std::move(agentId)), agentPath_(std::move(agentPath)), agentProcess_(std::move(agentProcess)), agentProcessOptions_(std::move(agentProcessOptions)){

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

