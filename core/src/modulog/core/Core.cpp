#include <modulog/core/Core.hpp>

namespace modulog::core {
    Core::Core(std::shared_ptr<asio::io_context> ioContext,
               std::shared_ptr<meta_lib::SharedSettings> sharedSettings) :
            sendAliveTimer_(*ioContext),
            server_(*ioContext, messageMutex_, messageConditionVar_, totalReceivedMessages_, sharedSettings),
            sharedSettings_(sharedSettings) {
        ioContext_ = ioContext;
        agentHandler_ = std::make_shared<AgentHandler>(std::filesystem::absolute(sharedSettings_->LogSettings.enabledAgentsPath));
    }

    void Core::start() {
        // start server:
        #ifdef BRINGAUTO_TESTS
            sharedSettings_->Testing.transitions->goToState("Start");
        #endif
        server_.startAccept();
        serverThread_ = std::thread{[this]() { ioContext_->run(); }};
        #ifdef BRINGAUTO_TESTS
            sharedSettings_->Testing.transitions->goToState("ServerCreated");
        #endif
        try {
            initAllAgents();
        } catch (std::exception &e) {
            cleanAll();
            throw;
        }
        notifyAllAgentsToSendLogs();
        startSendAlive();
        LogSaver logSaver(sharedSettings_->LogSettings.logsDestination);
        while (!stopFlag.load() && !agentHandler_->getRunningAgents().empty()) {
            #ifdef BRINGAUTO_TESTS
                if(!stopFlag.load())
                        sharedSettings_->Testing.transitions->goToState("WaitForMessage");
            #endif
            {
                std::unique_lock<std::mutex> lck(messageMutex_);
                messageConditionVar_.wait(lck, [this] { return totalReceivedMessages_; });
                totalReceivedMessages_--;
            }
            #ifdef BRINGAUTO_TESTS
                if(!stopFlag.load())
                        sharedSettings_->Testing.transitions->goToState("ProcessMessage");
            #endif
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
                    logSaver.saveLog(actAgent->getId(), logMsg);
                }
            }
            for (auto &del: agentsToDel) {
                agentHandler_->deleteAgent(del);
            }
            agentsToDel.clear();
        }
        #ifdef BRINGAUTO_TESTS
            sharedSettings_->Testing.transitions->goToState("Exiting");
        #endif
        cleanAll();
    }


    void Core::startSendAlive() {
        sendAliveTimer_.expires_from_now(std::chrono::seconds(sharedSettings_->LogSettings.isAliveIntervalSec));
        sendAliveTimer_.async_wait([this](const asio::error_code &) { sendAlive(); });
    }


    void Core::sendAlive() {
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
        std::vector<std::shared_ptr<Agent>> agentsToDel;
        for (auto &agent: agentHandler_->getRunningAgents()) {
            if (!agent->getConfirmedAlive()) {
                bringauto::logging::Logger::logError("Agent {} didn't respond on IS_ALIVE!", agent->getId());
                agentsToDel.push_back(agent);
            } else {
                agent->setConfirmedAlive(false);
            }
        }
        for (auto &toDel: agentsToDel) {
            agentHandler_->deleteAgent(toDel); //TODO create from two for loops one (with deleting inside)
            { // if is last agent killed, program freezes - waits for other messages. This simulates message receive
                std::lock_guard<std::mutex> lck(messageMutex_);
                totalReceivedMessages_++;
                messageConditionVar_.notify_one();
            }
        }
        startSendAlive();
    }

    void Core::initAllAgents() {
        std::shared_ptr<AgentProcess> agentInfo;
        while ((agentInfo = agentHandler_->runNextAgent()) != nullptr) {
            #ifdef BRINGAUTO_TESTS
                sharedSettings_->Testing.transitions->goToState("CreatingAgent");
            #endif
            bringauto::logging::Logger::logInfo("waiting for ag. connection...");
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
            std::shared_ptr<communication::ControlMessage> respControlMessage = messageExchanger->waitForControlMessage(
                    2000);
            if (respControlMessage == nullptr)
                throw std::runtime_error("null control msg");

            //Now expecting ACK response with agent name:
            std::string agentName = respControlMessage->getValue();
            if(agentName.empty())
                throw std::runtime_error("Agent didn't send name!");
            agentInfo->setAgentId(agentName);
            agentHandler_->addNewAgent(messageExchanger, agentInfo);
        }
    }


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
            #ifdef BRINGAUTO_TESTS
                sharedSettings_->Testing.transitions->goToState("StopAgent");
            #endif
            agentHandler_->deleteAgent(toDel);
        }
        ioContext_->stop();
        serverThread_.join();
        sendAliveTimer_.cancel();
        #ifdef BRINGAUTO_TEST
            sharedSettings_->Testing.transitions->goToState("CleanExit");
        #endif
    }


}
