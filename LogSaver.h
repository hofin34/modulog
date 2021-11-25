#pragma once

#include <filesystem>
#include <fstream>
#include "LogMessage.h"

class LogSaver {
public:
    LogSaver(std::filesystem::path logsPath) : logsPath_(logsPath){};
    void saveLog(const std::string& agentId, const std::shared_ptr<LogMessage>& logMessage);
private:
    std::filesystem::path logsPath_;

};
