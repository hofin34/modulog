//
// Created by martin on 22.11.21.
//

#include "Core.h"
#include "TcpServer.h"
#include "MessageDeserializer.h"
#include "LogSaver.h"

Core::Core(const std::filesystem::path& pathToAgentsConfigs, std::shared_ptr<asio::io_context> ioContext) : agentHandler_(pathToAgentsConfigs),
                                                                                                            sendAliveTimer_(*ioContext){
    ioContext_ = ioContext;
}



void Core::start() {
    try
    {
        // start server:
       // asio::io_context ioContext;
        TcpServer server(*ioContext_);
        server.start_accept();
        std::thread serverThread{[this](){ ioContext_->run(); }};

        //TODO del:

       // serverThread.join();

        //Creates agent:
        std::shared_ptr<Agent> agent;
        while((agent = agentHandler_.createNextAgent()) != nullptr){
            asio::steady_timer timer(*ioContext_);
            std::cout << "waiting for ag. connection..." << std::endl;
            timer.expires_from_now(asio::chrono::seconds(2)); // TODO make variable time
            timer.wait();
            std::cout << "timer out..." << std::endl;
            auto agentConnection = server.popConnection();
            agent->setConnection(agentConnection);
            if(agentConnection == nullptr)
                throw std::runtime_error("Agent didnt connect.");
            agentConnection->start_read();

            //TODO check if it is agent:
            auto controlMessage = std::make_shared<ControlMessage>(ControlMessage::CONTROL_MSG_TYPE::CONFIG, agent->getConfig().dump());
            MessageSerializer msgSerializer(controlMessage);
            std::string toSend = msgSerializer.serialize();
            agentConnection->send_message(toSend);
            std::cout << "Waiting for agent response..." << std::endl;
            std::shared_ptr<ControlMessage> respControlMessage;
            while((respControlMessage = agent->popControlMessage()) == nullptr); // Waiting for response from agent
            //Now expecting ACK response with agent name...
            std::string agentName = respControlMessage->getValue();
            std::cout << "Core received.: " << agentName << std::endl;
            if(agentName != agent->getId()){
                std::cerr << "Agent must response with its name! "; // TODO pop agent/reset/something
            }


            //TODO start send alive timer (async)
            startSendAlive();
            LogSaver logSaver("../logs");
            while(true){
                for(auto &actAgent : agentHandler_.getRunningAgents()){
                    auto logMsg = actAgent->popLogMessage();
                    if(logMsg != nullptr){
                        logSaver.saveLog(actAgent->getId(), logMsg);
                        std::cout << "CORE received:" << logMsg->getValue() << std::endl;
                    }

                    auto controlMsg = actAgent->popControlMessage();
                    if(controlMsg != nullptr){
                        if(controlMsg->getType() == ControlMessage::CONTROL_MSG_TYPE::ACK){
                            actAgent->setConfirmedAlive(true);
                        }
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
    std::cout << "Core sending isAlive to all agents..." << std::endl;
    for(auto& agent: agentHandler_.getRunningAgents()){
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
    for(auto& agent: agentHandler_.getRunningAgents()){
        if(!agent->getConfirmedAlive()){
            std::cerr << "Agent " << agent->getId() << " didn't respond on isAlive!" << std::endl; // TODO kill or something...
        }else{
            agent->setConfirmedAlive(false);
        }
    }
    startSendAlive();
}
