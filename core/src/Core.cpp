//
// Created by martin on 22.11.21.
//

#include "../include/Core.h"
#include "../../communication/include/TcpServer.h"
#include "../../communication/include/MessageDeserializer.h"
#include "../include/LogSaver.h"

Core::Core(const std::filesystem::path& pathToAgentsConfigs, std::shared_ptr<asio::io_context> ioContext) :
                                                                                                            sendAliveTimer_(*ioContext),
                                                                                                            server_(*ioContext, messageMutex_, messageConditionVar_, totalReceivedMessages_){
    ioContext_ = ioContext;
    agentHandler_ = std::make_shared<AgentHandler>(pathToAgentsConfigs);
}

void Core::start() {
    try
    {
        // start server:
        server_.start_accept();
        std::thread serverThread{[this](){ ioContext_->run(); }};
        initAllAgents();
        notifyAllAgentsToSendLogs();
        startSendAlive();
        LogSaver logSaver("../logs");
        while(true) {
            std::cout << "NEW ITER" << std::endl;
            {
                std::unique_lock<std::mutex> lck(messageMutex_);
                messageConditionVar_.wait(lck, [this]{ return totalReceivedMessages_; });
                totalReceivedMessages_--;
            }
            for (auto &actAgent: agentHandler_->getRunningAgents()) {
                auto logMsg = actAgent->getTcpConnection()->getMessageProcessor_()->popLogMessage();
                if(logMsg != nullptr){
                    std::cout << "LOG:" << logMsg->getValue() << std::endl;
                    logSaver.saveLog(actAgent->getId(), logMsg);
                }
                auto controlMsg = actAgent->getTcpConnection()->getMessageProcessor_()->popControlMessage();
                if(controlMsg != nullptr){
                    if(controlMsg->getType() == ControlMessage::CONTROL_MSG_TYPE::ACK){
                        actAgent->setConfirmedAlive(true);
                    }else if(controlMsg->getType() == ControlMessage::CONTROL_MSG_TYPE::EXIT){
                        std::cerr << "Agent wants to exit..." << std::endl;
                    }
                }
            }
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
    sendAliveTimer_.expires_from_now(std::chrono::seconds(10));
    sendAliveTimer_.async_wait(std::bind(&Core::sendAlive, this));
}

void Core::sendAlive() {
    std::cout << "Core sending isAlive to all agents..." << std::endl; //TODO when sending to dead agent, core exits...
    for(auto& agent: agentHandler_->getRunningAgents()){
        auto isAliveMsg = std::make_shared<ControlMessage>(ControlMessage::CONTROL_MSG_TYPE::IS_ALIVE, "");
        MessageSerializer messageSerializer(isAliveMsg);
        std::string toSend = messageSerializer.serialize();
        agent->setConfirmedAlive(false);
        agent->getConnection()->send_message(toSend);
    }
    sendAliveTimer_.expires_from_now(std::chrono::seconds(2));
    sendAliveTimer_.async_wait(std::bind(&Core::checkIfAgentsAlive, this));
}

void Core::checkIfAgentsAlive() {
    std::cout << "Core checking if all agents responded with ACK..." << std::endl;
    for(auto& agent: agentHandler_->getRunningAgents()){
        if(!agent->getConfirmedAlive()){
            std::cerr << "Agent " << agent->getId() << " didn't respond on isAlive!" << std::endl; // TODO kill or something...
        }else{
            agent->setConfirmedAlive(false);
        }
    }
    startSendAlive();
}

void Core::initAllAgents() {
    std::shared_ptr<Agent> agent;
    while((agent = agentHandler_->createNextAgent()) != nullptr){
        std::cout << "waiting for ag. connection..." << std::endl;
        TcpConnection::pointer agentConnection;
        auto endConnectionTime = std::chrono::system_clock::now() + std::chrono::seconds(3); // TODO 3 seconds to variable
        while((endConnectionTime > std::chrono::system_clock::now()) && (agentConnection = server_.popConnection()) == nullptr); //TODO wait timer
        if(agentConnection == nullptr){
            std::cerr << "Agent " << agent->getId() << " didn't connect!";
            agentHandler_->deleteAgent(agent);
            continue;
        }
        agent->setConnection(agentConnection);
        agentConnection->start_read();

        //TODO check if it is agent:
        auto controlMessage = std::make_shared<ControlMessage>(ControlMessage::CONTROL_MSG_TYPE::CONFIG, "agent->getConfig().dump()");
        MessageSerializer msgSerializer(controlMessage);
        agentConnection->send_message(msgSerializer.serialize());
        std::cout << "Waiting for agent response..." << std::endl;
        std::shared_ptr<ControlMessage> respControlMessage = agentConnection->getMessageProcessor_()->waitForControlMessage(); // TODO if someone else sends message...
        if(respControlMessage == nullptr){
            std::cout << "null control msg" << std::endl;
            exit(2); //TODO - timeout
        }
        //Now expecting ACK response with agent name:
        std::string agentName = respControlMessage->getValue();
        std::cout << "Core received.: " << agentName << std::endl;
        agent->setId(agentName); // TODO if sends empty?
    }
}

void Core::notifyAllAgentsToSendLogs() {
    for (auto &actAgent: agentHandler_->getRunningAgents()) {
        auto startSendMsg = std::make_shared<ControlMessage>(ControlMessage::CONTROL_MSG_TYPE::ACK, "");
        MessageSerializer msgSerializer(startSendMsg);
        actAgent->getTcpConnection()->send_message(msgSerializer.serialize());
    }
}

