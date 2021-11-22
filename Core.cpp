//
// Created by martin on 22.11.21.
//

#include "Core.h"
#include "TcpServer.h"

void Core::start() {
    try
    {

        // start server:
        asio::io_context io_context;
        TcpServer server(io_context);
        server.start_accept();
        std::thread serverThread{[&io_context](){ io_context.run(); }};
        serverThread.join(); //TODO remove

        //Creates agent:
        std::shared_ptr<Agent> agent = agentHandler.createNextAgent();
        asio::steady_timer timer(io_context);
        std::cout << "waiting for ag. connection..." << std::endl;
        timer.expires_from_now(asio::chrono::seconds(2)); // TODO make variable time
        timer.wait();
        std::cout << "timer out..." << std::endl;
        auto agentConnection = server.popConnection();
        if(agentConnection == nullptr)
            throw std::runtime_error("Agent didnt connect.");
        //TODO check if it is agent:
        ControlMessage controlMessage(ControlMessage::CONTROL_MSG_TYPE::IS_ALIVE, "");
        std::string toSend = controlMessage.serialize();
        agentConnection->send_message(toSend);
        std::cout << "Waiting for agent response..." << std::endl;
        while(agentConnection->getMessagesVector().empty()); // Waiting for response from agent
        std::string response = agentConnection->getFrontMessage();
        agentConnection->popMessage();
        std::cout << "Response: " << response << std::endl;


        //std::thread thread1{[&io_context](){ io_context.run(); }};
        //std::thread thread2{[&io_context](){ io_context.run(); }};
        //thread1.join();
        serverThread.join();

        //io_context.run();
    }
    catch (std::exception& e)
    {
        std::cerr << e.what() << std::endl;
    }
}
