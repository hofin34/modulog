#include <modulog/core/Core.hpp>

namespace modulog::core {
    Core::Core(const std::filesystem::path &pathToEnabledAgentsList, std::shared_ptr<asio::io_context> ioContext,
               std::shared_ptr<communication::SharedSettings> sharedSettings) :
            sendAliveTimer_(*ioContext),
            server_(*ioContext, messageMutex_, messageConditionVar_, totalReceivedMessages_, sharedSettings),
            sharedSettings_(sharedSettings) {
        ioContext_ = ioContext;
        agentHandler_ = std::make_shared<AgentHandler>(pathToEnabledAgentsList);
    }

    void Core::start() {
        // start server:
        server_.startAccept();
        serverThread_ = std::thread{[this]() { ioContext_->run(); }};
        try {
            initAllAgents();
        } catch (std::exception &e) {
            cleanAll();
            std::cerr << e.what() << std::endl;
            throw e; //TODO in main, priting this rethrow prints std::exception and not message
        }
        notifyAllAgentsToSendLogs();
        startSendAlive();
        LogSaver logSaver(sharedSettings_->LogSettings.logsDestination);
        while (!stopFlag.load() && !agentHandler_->getRunningAgents().empty()) {
            std::cout << "NEW ITER" << std::endl;
            {
                std::unique_lock<std::mutex> lck(messageMutex_);
                messageConditionVar_.wait(lck, [this] { return totalReceivedMessages_; });
                totalReceivedMessages_--;
            }
            std::vector<std::shared_ptr<Agent>> agentsToDel;
            int i = 0;
            for (auto &actAgent: agentHandler_->getRunningAgents()) {
                auto controlMsg = actAgent->getMessageExchanger()->popControlMessage();
                if (controlMsg != nullptr) {
                    if (controlMsg->getType() == communication::ControlMessage::CONTROL_MSG_TYPE::EXIT) {
                        std::cerr << "Agent " << actAgent->getId() << " wants to exit..." << std::endl;
                        auto exitControlMsg = std::make_shared<communication::ControlMessage>(
                                communication::ControlMessage::CONTROL_MSG_TYPE::EXIT_ACK, "");
                        actAgent->getMessageExchanger()->sendControl(exitControlMsg);
                        agentsToDel.push_back(actAgent);
                        continue;
                    } else if (controlMsg->getType() == communication::ControlMessage::CONTROL_MSG_TYPE::EXIT_ERR) {
                        std::cerr << "Core received EXIT_ERR from " << actAgent->getId() << std::endl;
                        agentsToDel.push_back(actAgent);
                        continue;
                    } else if (controlMsg->getType() == communication::ControlMessage::CONTROL_MSG_TYPE::ACK) {
                        actAgent->setConfirmedAlive(true);
                    }
                }
                auto logMsg = actAgent->getMessageExchanger()->popLogMessage();
                if (logMsg != nullptr) {
                    std::cout << "LOG:" << logMsg->getValue() << std::endl;
                    logSaver.saveLog(actAgent->getId(), logMsg);
                }
            }
            for (auto &del: agentsToDel) {
                agentHandler_->deleteAgent(del);
            }
            agentsToDel.clear();
        }
        cleanAll();
    }


    void Core::startSendAlive() {
        sendAliveTimer_.expires_from_now(std::chrono::seconds(sharedSettings_->LogSettings.isAliveIntervalSec));
        sendAliveTimer_.async_wait([this](const asio::error_code &) { sendAlive(); });
    }


    void Core::sendAlive() {
        std::cout << "Core sending isAlive to all agents..." << std::endl;
        for (auto &agent: agentHandler_->getRunningAgents()) {
            auto isAliveMsg = std::make_shared<communication::ControlMessage>(
                    communication::ControlMessage::CONTROL_MSG_TYPE::IS_ALIVE, "");
            agent->setConfirmedAlive(false);
            agent->getMessageExchanger()->sendControl(isAliveMsg);
        }
        sendAliveTimer_.expires_from_now(std::chrono::seconds(sharedSettings_->LogSettings.isAliveTimeoutSec));
        sendAliveTimer_.async_wait([this](const asio::error_code &) { checkIfAgentsAlive(); });
    }

    void Core::checkIfAgentsAlive() {
        std::cout << "Core checking if all agents responded with ACK..." << std::endl;
        std::vector<std::shared_ptr<Agent>> agentsToDel;
        for (auto &agent: agentHandler_->getRunningAgents()) {
            if (!agent->getConfirmedAlive()) {
                std::cerr << "Agent " << agent->getId() << " didn't respond on isAlive!" << std::endl;
                agentsToDel.push_back(agent);
            } else {
                agent->setConfirmedAlive(false);
            }
        }
        for (auto &toDel: agentsToDel) {
            agentHandler_->deleteAgent(toDel); //TODO create from two for loops one (with deleting inside)
        }
        startSendAlive();
    }

    void Core::initAllAgents() {
        std::shared_ptr<AgentProcess> agentInfo;
        while ((agentInfo = agentHandler_->runNextAgent()) != nullptr) {
            std::cout << "waiting for ag. connection..." << std::endl;
            std::shared_ptr<communication::TcpConnection> agentConnection;
            auto endConnectionTime =
                    std::chrono::system_clock::now() + std::chrono::seconds(sharedSettings_->ServerSettings.connectionTimeoutSec);
            while ((endConnectionTime > std::chrono::system_clock::now()) &&
                   (agentConnection = server_.popConnection()) == nullptr) {
                std::this_thread::sleep_for(std::chrono::microseconds(
                        20));// sleep for valgrind, if not set, agent will not connect in valgrind environment...
            };
            if (agentConnection == nullptr) {
                agentInfo->stopAgent();
                throw std::runtime_error("Agent " + agentInfo->getAgentId() + " didn't connect!");
            }

            auto messageExchanger = std::make_shared<communication::MessageExchanger>(agentConnection);

            std::string configString = "NO-CONFIG";
            std::ifstream configStream(sharedSettings_->LogSettings.sharedAgentsConfig);
            if (configStream.is_open()) {
                std::stringstream configBuffer;
                configBuffer << configStream.rdbuf();
                configString = configBuffer.str();
            }
            auto controlMessage = std::make_shared<communication::ControlMessage>(
                    communication::ControlMessage::CONTROL_MSG_TYPE::CONFIG, configString);
            messageExchanger->sendControl(controlMessage);
            std::cout << "Waiting for agent response..." << std::endl;
            std::shared_ptr<communication::ControlMessage> respControlMessage = messageExchanger->waitForControlMessage(
                    2000);
            if (respControlMessage == nullptr)
                throw std::runtime_error("null control msg");

            //Now expecting ACK response with agent name:
            std::string agentName = respControlMessage->getValue();
            std::cout << "Core agent name.: " << agentName << std::endl;
            if(agentName.empty())
                throw std::runtime_error("Agent didn't send name!");
            agentInfo->setAgentId(agentName);
            agentHandler_->addNewAgent(messageExchanger, agentInfo);
        }
    }

//TODO error "pure virtual method called" occured one time, when two agents running!

    void Core::notifyAllAgentsToSendLogs() {
        for (auto &actAgent: agentHandler_->getRunningAgents()) {
            auto startSendMsg = std::make_shared<communication::ControlMessage>(
                    communication::ControlMessage::CONTROL_MSG_TYPE::ACK, "");
            actAgent->getMessageExchanger()->sendControl(startSendMsg);
        }
    }

    void Core::stop() {
        stopFlag = true;
    }

    void Core::cleanAll() {
        std::vector<std::shared_ptr<Agent>> agentsToDel;
        for (auto &toDel: agentHandler_->getRunningAgents()) {
            agentsToDel.push_back(toDel); // Cannot delete from inside for range - its removing from vector
        }
        for (auto &toDel: agentsToDel) {
            agentHandler_->deleteAgent(toDel);
        }
        ioContext_->stop();
        serverThread_.join();
        sendAliveTimer_.cancel();
    }


}
