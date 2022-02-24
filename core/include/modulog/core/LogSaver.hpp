#pragma once

#include <filesystem>
#include <fstream>
#include <modulog/communication/LogMessage.hpp>

namespace modulog::core{
    class LogSaver {
    public:
        /**
         * @param logsPath Path, where to save logs
         */
        LogSaver(std::filesystem::path logsPath) : logsPath_(logsPath){};
        /**
         * Save log (write it to file)
         * @param agentId From which agent log received
         * @param logMessage Which log to save
         */
        void saveLog(const std::string& agentId, const std::shared_ptr<communication::LogMessage>& logMessage);

        /**
         * Function saves crashed agent name into file.
         * @param agentId name of crashed agent
         */
        void logAgentCrash(std::string agentId);

    private:
        /**
         * If received log is of ERROR type, this function is called - creates directory for error logs and saves it
         * @param agentId From which agent log received
         * @param logMessage Which log to save
         */
        void saveErrorLog(const std::string& agentId, const std::shared_ptr<communication::LogMessage>& logMessage);
        std::filesystem::path logsPath_;

    };

}
