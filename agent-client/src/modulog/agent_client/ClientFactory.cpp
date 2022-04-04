#include <modulog/agent_client/ClientFactory.hpp>
#include <modulog/agent_client/StandaloneAgentClient.hpp>
#include <modulog/agent_client/TcpAgentClient.hpp>

namespace modulog::agent_client{

    std::shared_ptr<AgentClient> ClientFactory::createClient(std::shared_ptr<asio::io_context> &ioContext, const std::string& agentName) {

#ifdef AGENT_CLIENT_STANDALONE
        return std::make_shared<StandaloneAgentClient>(agentName);
#else
        return std::make_shared<TcpAgentClient>(ioContext, agentName);
#endif
    }
}
