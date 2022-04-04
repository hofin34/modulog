#include <modulog/agent_client/Helpers.hpp>

namespace modulog::agent_client{
    nlohmann::json Helpers::parseConfig(const std::filesystem::path &execPath) {
        std::filesystem::path agentPath = execPath;
        agentPath = agentPath.remove_filename();
        std::ifstream ifs(agentPath/"config.json");
        nlohmann::json configJson = nlohmann::json::parse(ifs);
        return configJson;
    }

    std::string Helpers::execCommand(const std::string& cmd) {
        std::array<char, 128> buffer{};
        std::string result;
        std::unique_ptr<FILE, decltype(&pclose)> pipe(popen(cmd.c_str(), "r"), pclose);
        if (!pipe) {
            throw std::runtime_error("popen() failed!");
        }
        while (fgets(buffer.data(), buffer.size(), pipe.get()) != nullptr) {
            result += buffer.data();
        }
        return result;
    }

    std::shared_ptr<communication::LogMessage> Helpers::createInfoLog(std::string key, std::string toLog) {
        return std::make_shared<modulog::communication::LogMessage>(
                modulog::communication::LogMessage::LOG_MSG_TYPE::LOG, key,
                toLog);
    }

    std::shared_ptr<communication::LogMessage> Helpers::createErrLog(std::string key, std::string toLog) {
        return std::make_shared<modulog::communication::LogMessage>(
                modulog::communication::LogMessage::LOG_MSG_TYPE::ERROR, key,
                toLog);
    }


}
