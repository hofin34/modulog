#pragma once

#include <modulog/communication/MessageDeserializer.hpp>
#include <modulog/communication/TcpConnection.hpp>
#include <modulog/communication/MessageProcessor.hpp>
#include <modulog/communication/MessageExchanger.hpp>
#include <modulog/core/AgentProcess.hpp>

#include <reproc++/reproc.hpp>

#include <string>
#include <filesystem>
#include <utility>

namespace modulog::core {
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
        Agent(const std::shared_ptr<AgentProcess> &agentInfo,
              const std::shared_ptr<communication::MessageExchanger> &messageExchanger) :
                agentProcess_(agentInfo), messageExchanger_(messageExchanger) {}

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
        std::shared_ptr<communication::MessageExchanger> getMessageExchanger();

        /**
         * if is agent exiting by core, this should be marked as true to know, that agent didn't fail
         * @param expectedExit
         */
        void setExpectedExit(bool expectedExit);

        /**
         * Default value is false - use this function just if agent exited to get status of exit
         * @return true if agent had clean exit, false if error occured in agent.
         */
        bool getExpectedExit();

    private:
        bool expectedExit_ = false;
        bool confirmedAlive_ = false;
        std::shared_ptr<communication::MessageExchanger> messageExchanger_;
        std::shared_ptr<AgentProcess> agentProcess_;
    };
}
