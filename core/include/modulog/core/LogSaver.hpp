#pragma once

#include <modulog/communication/LogMessage.hpp>
#include <modulog/meta_lib/SharedSettings.hpp>

#include <bringauto/logging/Logger.hpp>

#include <filesystem>
#include <fstream>
#include <utility>

namespace modulog::core {
    class LogSaver {
    public:
        /**
         * @param logsPath Path, where to save logs
         * @param sharedSettings shared setting - it will read from this file, if should log in one file or folder structure
         */
        explicit LogSaver(std::shared_ptr<meta_lib::SharedSettings> sharedSettings) : sharedSettings_(std::move(sharedSettings)) {
            logsPath_ = sharedSettings_->LogSettings.logsDestination;
            oneFileLog = sharedSettings_->LogSettings.oneFileLog;
        };

        /**
         * Save log (write it to file)
         * @param agentId From which agent log received
         * @param logMessage Which log to save
         */
        void saveLog(const std::string &agentId, const std::shared_ptr<communication::LogMessage> &logMessage);

        /**
         * Function saves crashed agent name into file.
         * @param agentId name of crashed agent
         */
        void logAgentCrash(const std::string& agentId);

    private:
        /**
         * If received log is of ERROR type, this function is called - creates directory for error logs and saves it
         * @param agentId From which agent log received
         * @param logMessage Which log to save
         */
        void saveErrorLog(const std::string &agentId, const std::shared_ptr<communication::LogMessage> &logMessage);

        std::filesystem::path logsPath_;
        bool oneFileLog = false;
        std::shared_ptr<meta_lib::SharedSettings> sharedSettings_;

    };

}
