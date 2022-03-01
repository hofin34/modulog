#include <iostream>
#include <modulog/agent_client/AgentClient.hpp>
#include <modulog/agent_client/Helpers.hpp>
#include <modulog/watchdog_agent/UdpServer.h>
#include <modulog/watchdog_agent/WatchdogHandler.h>


int main(int argc, char **argv) {
    nlohmann::json jsonConfig = modulog::agent_client::Helpers::parseConfig(argv[0]);
    if (!jsonConfig.contains("port"))
        throw std::runtime_error("No port in json config!");
    int port = jsonConfig["port"];

    if (!jsonConfig.contains("devices"))
        throw std::runtime_error("No devices key in json config!");
    if (jsonConfig["devices"].empty())
        throw std::runtime_error("No devices in json config!");

    std::vector<std::string> deviceVector;
    for (auto &device: jsonConfig["devices"]) {
        std::cout << "dev: " << device << std::endl;
        deviceVector.push_back(device);
    }
    try {
        asio::io_context ioContext;
        auto watchdogHandler = std::make_shared<modulog::watchdog_agent::WatchdogHandler>(ioContext);
        watchdogHandler->init(deviceVector);
        modulog::watchdog_agent::UdpServer server(ioContext, 6127, watchdogHandler);
        ioContext.run();
    }
    catch (std::exception &e) {
        std::cerr << e.what() << std::endl;
    }
    return 0;
}