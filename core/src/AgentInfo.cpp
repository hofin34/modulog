//
// Created by martin on 09.12.21.
//

#include "AgentInfo.h"

#include <utility>

void AgentInfo::setAgentId(std::string id) {
    agentId_ = std::move(id);
}
