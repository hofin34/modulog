#pragma once

#include <filesystem>
#include <fstream>
#include "../../communication/include/LogMessage.h"

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
    void saveLog(const std::string& agentId, const std::shared_ptr<LogMessage>& logMessage);
private:
    /**
     * If received log is of ERROR type, this function is called - creates directory for error logs and saves it
     * @param agentId From which agent log received
     * @param logMessage Which log to save
     */
    void saveErrorLog(const std::string& agentId, const std::shared_ptr<LogMessage>& logMessage);
    std::filesystem::path logsPath_;

};
