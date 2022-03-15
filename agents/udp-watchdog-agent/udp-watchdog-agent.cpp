#include <iostream>
#include <modulog/agent_client/AgentClient.hpp>
#include <modulog/agent_client/Helpers.hpp>
#include <modulog/watchdog_agent/UdpServer.hpp>
#include <modulog/watchdog_agent/WatchdogHandler.hpp>


int main(int argc, char **argv) {
    nlohmann::json jsonConfig = modulog::agent_client::Helpers::parseConfig(argv[0]);

    if (!jsonConfig.contains("id")){
        std::cerr << "No id in json config!" << std::endl;
        return EXIT_FAILURE;
    }
    std::string agentId = jsonConfig["id"];

    if (!jsonConfig.contains("port")){
        std::cerr << "No port in json config!" << std::endl;
        return EXIT_FAILURE;
    }
    int port = jsonConfig["port"];

    if (!jsonConfig.contains("devices")){
        std::cerr << "No devices key in json config!" << std::endl;
        return EXIT_FAILURE;
    }
    if (jsonConfig["devices"].empty()){
        std::cerr << "No devices in json config!" << std::endl;
        return EXIT_FAILURE;
    }
    if (!jsonConfig.contains("checkoutInterval")){
        std::cerr << "No checkoutInterval in json config!" << std::endl;
        return EXIT_FAILURE;
    }
    int checkoutInterval = jsonConfig["checkoutInterval"];


    std::vector<std::string> deviceVector;
    for (auto &device: jsonConfig["devices"]) {
        deviceVector.push_back(device);
    }
    try {
        auto ioContext = std::make_shared<asio::io_context>();
        auto agentClient = std::make_shared<modulog::agent_client::AgentClient>(ioContext, agentId);
        agentClient->initClient();
        auto watchdogHandler = std::make_shared<modulog::watchdog_agent::WatchdogHandler>(ioContext, checkoutInterval, agentClient);
        watchdogHandler->init(deviceVector);
        modulog::watchdog_agent::UdpServer server(ioContext, 6127, watchdogHandler);
        ioContext->run();
    }
    catch (std::exception &e) {
        std::cerr << "udp-watchdog-agent exception: " << e.what() << std::endl;
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}