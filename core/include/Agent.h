#pragma once


#include <string>
#include <filesystem>
#include <reproc++/reproc.hpp>
#include <utility>
#include "../../communication/include/TcpConnection.h"
#include "MessageProcessor.h"
#include "MessageExchanger.h"
#include "AgentInfo.h"

/**
 * Class representing agent - contains running instance of agent in AgentInfo and contains also
 * meta information about agent, like if he confirmed isAlive message etc.
 */
class Agent {
public:
    /**
     *
     * @param agentInfo representing running agent process
     * @param messageExchanger for sending and receiving messages from/to agent
     */
    Agent(const std::shared_ptr<AgentInfo> &agentInfo, const std::shared_ptr<MessageExchanger> &messageExchanger) :
            agentInfo_(agentInfo), messageExchanger_(messageExchanger) {}

    /**
     * Gets agent's id - when is agent process created, he sends message with this id and then is set.
     * @return agents id
     */
    std::string getId();

    /**
     * Setter for confirmedAlive_ variable
     * @param value true/false, if agent confirmed message isAlive or not
     */
    void setConfirmedAlive(bool value);

    /**
     * Getter
     * @return confirmedAlive_
     */
    bool getConfirmedAlive();

    /**
     * Cleaning after agent - closing connection and stopping agent process
     */
    void deleteSelf();

    /**
     * Getter
     * @return messageExchanger_
     */
    std::shared_ptr<MessageExchanger> getMessageExchanger();

private:
    std::string id_;
    bool confirmedAlive_ = false;
    std::shared_ptr<MessageExchanger> messageExchanger_;
    std::shared_ptr<AgentInfo> agentInfo_;
};