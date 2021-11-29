//
// Created by martin on 24.11.21.
//

#include "../include/LogSaver.h"

void LogSaver::saveLog(const std::string& agentId, const std::shared_ptr<LogMessage>& logMessage) {
    std::filesystem::path whereSave = logsPath_ / agentId;
    if(!std::filesystem::exists(whereSave))
        try{
            std::filesystem::create_directories(whereSave);
        }catch(std::exception& e){
            std::cerr << e.what() << std::endl;
            throw std::runtime_error(e.what());
        }
    std::ofstream logFile;
    std::string logFileName = logMessage->getKey() + ".txt";
    std::filesystem::path logFilePath = whereSave / logFileName;
    logFile.open(logFilePath, std::ios_base::app);
    logFile << "[" << logMessage->getTimestamp() << "] " << logMessage->getValue() << std::endl;
    logFile.close();
}
