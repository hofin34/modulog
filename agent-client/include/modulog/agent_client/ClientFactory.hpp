#pragma once

#include <memory>
#include <modulog/agent_client/AgentClient.hpp>
#include <asio.hpp>

namespace modulog::agent_client{
    class ClientFactory {
    public:
        static std::shared_ptr<AgentClient> createClient(std::shared_ptr<asio::io_context> &ioContext, const std::string& agentName);
    };
}

