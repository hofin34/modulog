#include <modulog/core/Core.hpp>

namespace modulog::core{
    Core::Core(const std::filesystem::path& pathToEnabledAgentsList, std::shared_ptr<asio::io_context> ioContext) :
            sendAliveTimer_(*ioContext),
            server_(*ioContext, messageMutex_, messageConditionVar_, totalReceivedMessages_){
        ioContext_ = ioContext;
        agentHandler_ = std::make_shared<AgentHandler>(pathToEnabledAgentsList);
    }

    void Core::start() {
        try
        {
            // start server:
            server_.startAccept();
            std::thread serverThread{[this](){ ioContext_->run(); }};
            initAllAgents();
            notifyAllAgentsToSendLogs();
            //startSendAlive(); // TODO turn on
            LogSaver logSaver("../logs"); //TODO MOVE to global config
            while(true) {
                std::cout << "NEW ITER" << std::endl;
                {
                    std::unique_lock<std::mutex> lck(messageMutex_);
                    messageConditionVar_.wait(lck, [this]{ return totalReceivedMessages_; });
                    totalReceivedMessages_--;
                }
                std::vector<std::shared_ptr<Agent>> agentsToDel;
                int i = 0;
                for(auto &actAgent : agentHandler_->getRunningAgents()){
                    auto controlMsg = actAgent->getMessageExchanger()->popControlMessage();
                    if(controlMsg != nullptr){
                        if(controlMsg->getType() == communication::ControlMessage::CONTROL_MSG_TYPE::EXIT){
                            std::cerr << "Agent " << actAgent->getId() << " wants to exit..." << std::endl;
                            auto exitControlMsg = std::make_shared<communication::ControlMessage>(communication::ControlMessage::CONTROL_MSG_TYPE::EXIT_ACK, "");
                            actAgent->getMessageExchanger()->sendControl(exitControlMsg);
                            agentsToDel.push_back(actAgent);
                            continue;
                        }else if(controlMsg->getType() == communication::ControlMessage::CONTROL_MSG_TYPE::EXIT_ERR){
                            std::cerr << "Core received EXIT_ERR from " << actAgent->getId() << std::endl;
                            //agentHandler_->deleteAgent(actIterator);
                            agentsToDel.push_back(actAgent);
                            continue;
                        }else if(controlMsg->getType() == communication::ControlMessage::CONTROL_MSG_TYPE::ACK){
                            actAgent->setConfirmedAlive(true);
                        }
                    }
                    auto logMsg = actAgent->getMessageExchanger()->popLogMessage();
                    if(logMsg != nullptr){
                        std::cout << "LOG:" << logMsg->getValue() << std::endl;
                        logSaver.saveLog(actAgent->getId(), logMsg);
                    }
                }
                for(auto &del : agentsToDel){
                    std::cout << "AAAAAAA:" <<del->getId() << std::endl;
                    agentHandler_->deleteAgent(del);
                }
                agentsToDel.clear();
            }
            serverThread.join();
        }
        catch (std::exception& e)
        {
            std::cout << "Exc.:" << e.what() << std::endl;
        }
        catch(...){
            std::cout << "Something bad occurred." << std::endl;
        }
    }


    void Core::startSendAlive() {
        sendAliveTimer_.expires_from_now(std::chrono::seconds(10)); //TODO MOVE to config file...
        sendAliveTimer_.async_wait([this](const asio::error_code&){sendAlive();});
    }


    void Core::sendAlive() {
        std::cout << "Core sending isAlive to all agents..." << std::endl; //TODO when sending to dead agent, core exits...
        for(auto& agent: agentHandler_->getRunningAgents()){
            auto isAliveMsg = std::make_shared<communication::ControlMessage>(communication::ControlMessage::CONTROL_MSG_TYPE::IS_ALIVE, "");
            agent->setConfirmedAlive(false);
            agent->getMessageExchanger()->sendControl(isAliveMsg);
        }
        sendAliveTimer_.expires_from_now(std::chrono::seconds(2));
        sendAliveTimer_.async_wait([this](const asio::error_code&){checkIfAgentsAlive();});
    }

    void Core::checkIfAgentsAlive() {
        std::cout << "Core checking if all agents responded with ACK..." << std::endl;
        for(auto& agent: agentHandler_->getRunningAgents()){
            if(!agent->getConfirmedAlive()){
                std::cerr << "Agent " << agent->getId() << " didn't respond on isAlive!" << std::endl;
                agentHandler_->deleteAgent(agent);
            }else{
                agent->setConfirmedAlive(false);
            }
        }
        startSendAlive();
    }

    void Core::initAllAgents() {
        std::shared_ptr<AgentProcess> agentInfo;
        while((agentInfo = agentHandler_->runNextAgent()) != nullptr){
            std::cout << "waiting for ag. connection..." << std::endl;
            communication::TcpConnection::pointer agentConnection;
            auto endConnectionTime = std::chrono::system_clock::now() + std::chrono::seconds(3); // TODO 3 seconds to variable
            while((endConnectionTime > std::chrono::system_clock::now()) && (agentConnection = server_.popConnection()) == nullptr){
                std::this_thread::sleep_for(std::chrono::microseconds(20));// sleep for valgrind, if not set, agent will not connect in valgrind environment...
            };
            if(agentConnection == nullptr){
                std::cerr << "Agent " << agentInfo->getAgentId() << " didn't connect!" << std::endl;
                agentInfo->stopAgent();
                continue;
            }
            auto messageExchanger = std::make_shared<communication::MessageExchanger>(agentConnection);


            //TODO check if it is agent:
            auto controlMessage = std::make_shared<communication::ControlMessage>(communication::ControlMessage::CONTROL_MSG_TYPE::CONFIG, "agent->getConfig().dump()"); //TODO MOVE to config - all agents will receive some same configurable info
            messageExchanger->sendControl(controlMessage);
            std::cout << "Waiting for agent response..." << std::endl;
            std::shared_ptr<communication::ControlMessage> respControlMessage = messageExchanger->waitForControlMessage(2000);
            if(respControlMessage == nullptr){
                std::cerr << "null control msg" << std::endl;
                exit(EXIT_FAILURE);
            }
            //Now expecting ACK response with agent name:
            std::string agentName = respControlMessage->getValue();
            std::cout << "Core agent name.: " << agentName << std::endl;
            agentInfo->setAgentId(agentName); // TODO if sends empty?
            agentHandler_->addNewAgent(messageExchanger, agentInfo);
        }
    }

//TODO error "pure virtual method called" occured one time, when two agents running!

    void Core::notifyAllAgentsToSendLogs() {
        for (auto &actAgent: agentHandler_->getRunningAgents()) {
            auto startSendMsg = std::make_shared<communication::ControlMessage>(communication::ControlMessage::CONTROL_MSG_TYPE::ACK, "");
            actAgent->getMessageExchanger()->sendControl(startSendMsg);
        }
    }

    void Core::stop() {
        std::cout << "TODO exit " << agentHandler_->getRunningAgents().size() << "agents + cleanup" << std::endl;
    }


}
