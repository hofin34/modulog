//
// Created by martin on 22.11.21.
//

#include "Core.h"
#include "TcpServer.h"
#include "MessageDeserializer.h"

void Core::start() {
    try
    {

        // start server:
        asio::io_context io_context;
        TcpServer server(io_context);
        server.start_accept();
        std::thread serverThread{[&io_context](){ io_context.run(); }};

        //Creates agent:
        std::shared_ptr<Agent> agent = agentHandler.createNextAgent();
        asio::steady_timer timer(io_context);
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
        auto controlMessage = std::make_shared<ControlMessage>(ControlMessage::CONTROL_MSG_TYPE::CONFIG, "SomeConfigJSON...");
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
                agent->setId(agentName);
            }
        }
        //TODO start send alive timer (async)
        while(true){
            for(auto &actAgent : agentHandler.getRunningAgents()){
                auto actAgentConnection = actAgent->getConnection();
                if(actAgentConnection->isMessage()){
                    auto frontMsg = actAgentConnection->popMessage();
                    std::cout << "CORE received:" << std::endl;
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

