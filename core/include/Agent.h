#pragma once


#include <string>
#include <filesystem>
#include "../../cmake-build-debug/_deps/reproc++-src/reproc++/include/reproc++/reproc.hpp"
#include "../../communication/include/TcpConnection.h"
#include "MessageProcessor.h"

class Agent {
public:
    Agent(std::string id, std::shared_ptr<reproc::process>& process):
            id_(std::move(id)), process_(process){
        processOptions_ = std::make_shared<reproc::options>();
        processOptions_->stop = {
                { reproc::stop::terminate, reproc::milliseconds(2000) }, // TODO how long???
                { reproc::stop::kill, reproc::milliseconds(0) }
        };
    }
    // -------
    std::string getId();
    void setId(const std::string& id);
    void setConnection(const TcpConnection::pointer& connection);
    TcpConnection::pointer getConnection();
    int getProcessPid();
    void setConfirmedAlive(bool value);
    bool getConfirmedAlive();
    TcpConnection::pointer getTcpConnection();
    void deleteSelf();
private:
    std::string id_;
    std::filesystem::path path_;
    std::shared_ptr<reproc::process> process_;
    std::shared_ptr<reproc::options> processOptions_;
    TcpConnection::pointer tcpConnection_;
    bool confirmedAlive_ = false;
};