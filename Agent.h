#pragma once


#include <string>
#include <filesystem>
#include <reproc++/reproc.hpp>
#include "TcpConnection.h"

class Agent {
public:
    Agent(std::string id, int terminate_timeout, std::shared_ptr<reproc::process>& process, nlohmann::json config):
            id_(std::move(id)), process_(process), config_(config) {
        processOptions_ = std::make_shared<reproc::options>();
        processOptions_->stop = {
                { reproc::stop::terminate, reproc::milliseconds(terminate_timeout) },
                { reproc::stop::kill, reproc::milliseconds(0) }
        };
    }
    // -------
    std::string getId();

    std::filesystem::path getPath();
    std::shared_ptr<reproc::process> getProcess();
    std::shared_ptr<reproc::options> getProcessOptions();
    void setConnection(const TcpConnection::pointer& connection);
    TcpConnection::pointer getConnection();
    int getProcessPid();
    nlohmann::json getConfig();

private:
    std::string id_;
    std::filesystem::path path_;
    std::shared_ptr<reproc::process> process_;
    std::shared_ptr<reproc::options> processOptions_;
    TcpConnection::pointer tcpConnection_;
    nlohmann::json config_;
};