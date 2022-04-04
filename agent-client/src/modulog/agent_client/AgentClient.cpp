#include <modulog/agent_client/AgentClient.hpp>
#include <csignal>


namespace modulog::agent_client {


    void AgentClient::generalInit(std::string agentName) {
        bringauto::logging::Logger::addSink<bringauto::logging::ConsoleSink>();
        bringauto::logging::Logger::LoggerSettings params{agentName,
                                                          bringauto::logging::Logger::Verbosity::Debug};
        bringauto::logging::Logger::init(params);

    }
}
