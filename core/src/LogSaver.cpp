//
// Created by martin on 24.11.21.
//

#include "../include/LogSaver.h"

void LogSaver::saveLog(const std::string& agentId, const std::shared_ptr<LogMessage>& logMessage) {
    std::filesystem::path whereSave = logsPath_ / agentId;
    if(!std::filesystem::exists(whereSave)){
        try{
            std::filesystem::create_directories(whereSave);
        }catch(std::exception& e){
            std::cerr << e.what() << std::endl;
            throw std::runtime_error(e.what());
        }
    }

    std::ofstream logFile;
    std::string logFileName = logMessage->getKey() + ".txt";
    std::filesystem::path logFilePath = whereSave / logFileName;
    logFile.open(logFilePath, std::ios_base::app);
    logFile << "[" << logMessage->getTimestamp() << "] " << logMessage->getValue() << " " << LogMessage::getTypeStr(logMessage->getType()) << std::endl;
    logFile.close();
    if(logMessage->getType() == LogMessage::LOG_MSG_TYPE::ERROR){
        saveErrorLog(agentId, logMessage);
    }
}

void LogSaver::saveErrorLog(const std::string& agentId, const std::shared_ptr<LogMessage> &logMessage) {
    std::filesystem::path errorsDir = logsPath_ / "errors" / agentId;
    bool firstError = false;
    if(!std::filesystem::exists(errorsDir)){
        try{
            std::filesystem::create_directories(errorsDir);
        }catch(std::exception& e){
            std::cerr << e.what() << std::endl;
            throw std::runtime_error(e.what());
        }
    }

    std::string logFileName = logMessage->getKey() + ".txt";
    std::filesystem::path logFilePath = errorsDir / logFileName;
    if(!std::filesystem::exists(logFilePath)){
        std::ofstream logFile(logFilePath, std::ios_base::app);
        logFile << "Agent " << agentId << " logged error on " <<logMessage->getTimestamp() << " with key=value: " << logMessage->getKey() << "=" << logMessage->getValue() << std::endl;
        logFile << "Here is logged just first error, there can be more. Look at agent folder for more logs." << std::endl;
        logFile.close();
    }else{
        std::cerr << "file exists." << std::endl;
    }

}
