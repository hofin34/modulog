#pragma once

#include <filesystem>
#include <fstream>

class LogSaver {
public:
    LogSaver(std::filesystem::path logsPath) : logsPath_(logsPath){};
    void saveLog(const std::string& agentId, const std::string& log);
private:
    std::filesystem::path logsPath_;

};
