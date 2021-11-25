//
// Created by martin on 24.11.21.
//

#include "LogSaver.h"

void LogSaver::saveLog(const std::string& agentId, const std::shared_ptr<LogMessage>& logMessage) {
    std::filesystem::path whereSave = logsPath_ / agentId;
    if(!std::filesystem::exists(whereSave))
        std::filesystem::create_directory(whereSave);
    std::ofstream logFile;
    std::string logFileName = agentId + ".txt";
    std::filesystem::path logFilePath = whereSave / logFileName;
    logFile.open(logFilePath, std::ios_base::app);
    logFile << logMessage->getValue();
    logFile.close();
}
