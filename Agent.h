#pragma once


#include <string>
#include <filesystem>
#include <reproc++/reproc.hpp>
#include "TcpConnection.h"

class Agent {
public:
    Agent(std::string name, int terminate_timeout, std::shared_ptr<reproc::process>& process):
            id_(std::move(name)), process_(process) {
        processOptions_ = std::make_shared<reproc::options>();
        processOptions_->stop = {
                { reproc::stop::terminate, reproc::milliseconds(terminate_timeout) },
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

private:
    std::string id_;
    std::filesystem::path path_;
    std::shared_ptr<reproc::process> process_;
    std::shared_ptr<reproc::options> processOptions_;
    TcpConnection::pointer tcpConnection_;
};