#pragma once


#include <string>
#include <filesystem>
#include "../../cmake-build-debug/_deps/reproc++-src/reproc++/include/reproc++/reproc.hpp"
#include "../../communication/include/TcpConnection.h"

class Agent {
public:
    Agent(std::string id, std::shared_ptr<reproc::process>& process):
            id_(std::move(id)), process_(process), logMessages_(), controlMessages_(){
        processOptions_ = std::make_shared<reproc::options>();
        processOptions_->stop = {
                { reproc::stop::terminate, reproc::milliseconds(2000) }, // TODO how long???
                { reproc::stop::kill, reproc::milliseconds(0) }
        };
    }
    // -------
    std::string getId();
    void setId(const std::string& id);
    std::filesystem::path getPath();
    std::shared_ptr<reproc::process> getProcess();
    std::shared_ptr<reproc::options> getProcessOptions();
    void setConnection(const TcpConnection::pointer& connection);
    TcpConnection::pointer getConnection();
    int getProcessPid();
    std::shared_ptr<LogMessage> popLogMessage();
    std::shared_ptr<ControlMessage> popControlMessage();
    void setConfirmedAlive(bool value);
    bool getConfirmedAlive();

private:
    std::string id_;
    std::filesystem::path path_;
    std::shared_ptr<reproc::process> process_;
    std::shared_ptr<reproc::options> processOptions_;
    TcpConnection::pointer tcpConnection_;
    std::vector<std::shared_ptr<LogMessage>> logMessages_;
    std::vector<std::shared_ptr<ControlMessage>> controlMessages_;
    void processMessages(int maxCount);
    bool confirmedAlive_ = false;
};