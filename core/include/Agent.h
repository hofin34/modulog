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
    Agent(const std::shared_ptr<AgentInfo> &agentInfo, const std::shared_ptr<MessageExchanger> &messageExchanger):
            agentInfo_(agentInfo), messageExchanger_(messageExchanger) {}
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
    bool confirmedAlive_ = false;
    std::shared_ptr<MessageExchanger> messageExchanger_;
    std::shared_ptr<AgentInfo> agentInfo_;
};