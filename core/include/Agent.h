#pragma once


#include <string>
#include <filesystem>
#include <reproc++/reproc.hpp>
#include <utility>
#include "../../communication/include/TcpConnection.h"
#include "MessageProcessor.h"
#include "MessageExchanger.h"
#include "AgentInfo.h"

class Agent {
public:
    Agent(std::shared_ptr<AgentInfo> agentInfo, std::shared_ptr<MessageExchanger> messageExchanger):
           messageExchanger_(std::move(messageExchanger)){
        processOptions_ = std::make_shared<reproc::options>();
        processOptions_->stop = {
                { reproc::stop::terminate, reproc::milliseconds(2000) }, // TODO how long???
                { reproc::stop::kill, reproc::milliseconds(0) }
        };
    }
    // -------
    std::string getId();
    void setId(const std::string& id);
    int getProcessPid();
    void setConfirmedAlive(bool value);
    bool getConfirmedAlive();
    void deleteSelf();
    std::shared_ptr<MessageExchanger> getMessageExchanger();
private:
    std::string id_;
    std::filesystem::path path_;
    std::shared_ptr<reproc::process> process_;
    std::shared_ptr<reproc::options> processOptions_;
    TcpConnection::pointer tcpConnection_;
    bool confirmedAlive_ = false;
    std::shared_ptr<MessageExchanger> messageExchanger_;
};