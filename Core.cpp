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
            while(!agentConnection->isMessage()); // Waiting for response from agent
            std::string response = agentConnection->getFrontMessage();
            agentConnection->popMessage();
            //Now expecting ACK response with agent name...
            std::cout << "Core received.: " << response << std::endl;
            MessageDeserializer messageDeserializer(response);
            if(messageDeserializer.getMsgType() == Message::MSG_TYPE::CONTROL_MSG){
                auto respControlMessage = messageDeserializer.getControlMessage();
                if(respControlMessage->getType() == ControlMessage::CONTROL_MSG_TYPE::ACK){
                    std::string agentName = respControlMessage->getValue();
                    if(agentName != agent->getId()){
                        std::cerr << "Agent must response with its name! "; // TODO pop agent/reset/something
                    }
                }
            }
            //TODO start send alive timer (async)
            startSendAlive();
            LogSaver logSaver("../logs");
            while(true){
                for(auto &actAgent : agentHandler_.getRunningAgents()){
                    auto actAgentConnection = actAgent->getConnection();
                    if(actAgentConnection->isMessage()){
                        auto frontMsg = actAgentConnection->popMessage();
                        MessageDeserializer deserializer (frontMsg);
                        if(deserializer.getMsgType() == Message::MSG_TYPE::LOG_MSG){
                            auto logMsg = deserializer.getLogMessage();
                            logSaver.saveLog(actAgent->getId(), logMsg->getValue());
                        }
                        std::cout << "CORE received:" << frontMsg << std::endl;
                    }
                }
            }
        }




        //std::thread thread1{[&io_context](){ io_context.run(); }};
        //std::thread thread2{[&io_context](){ io_context.run(); }};
        //thread1.join();
        serverThread.join();

        //io_context.run();
    }
    catch (std::exception& e)
    {
        std::cout << "Exc.:" << e.what() << std::endl;
    }
    catch(...){
        std::cout << "Something bad occured." << std::endl;
    }
}


void Core::startSendAlive() {
    /*asio::steady_timer timer(ioContext);
    timer.expires_from_now(std::chrono::seconds(5));*/


}
