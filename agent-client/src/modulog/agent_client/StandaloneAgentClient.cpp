#include <modulog/agent_client/StandaloneAgentClient.hpp>


namespace modulog::agent_client {

    void StandaloneAgentClient::sendLog(const std::shared_ptr<communication::LogMessage> &logMessage) {
        bringauto::logging::Logger::logInfo("Simulated log send (just debug): {}", logMessage->serialize());
    }

    void
    StandaloneAgentClient::sendControl(const std::shared_ptr<communication::ControlMessage> &controlMessage) {
        bringauto::logging::Logger::logInfo("Simulated log send (just debug): {}", controlMessage->serialize());
    }

    void StandaloneAgentClient::initClient() {
        generalInit(agentName_);
        bringauto::logging::Logger::logInfo("AGENT_CLIENT_STANDALONE is turned ON!");
    }

    void StandaloneAgentClient::exitConnection() {
        canLog_ = false;
    }

    std::string StandaloneAgentClient::getSharedConfig() {
        return "EMPTY_STANDALONE_SHARED_CONFIG";
    }

    bool StandaloneAgentClient::canLog() {
        return canLog_;
    }

    bool StandaloneAgentClient::sleepFor(std::chrono::seconds sleepTime) {
        std::this_thread::sleep_for(sleepTime);
        return true;
    }
}
