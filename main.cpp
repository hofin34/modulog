#include <iostream>
#include <modulog/core/Core.hpp>
#include <vector>
#include <asio.hpp>
#include <modulog/communication/SharedSettings.hpp>

// Just for debug, TODO remove later:
std::string ver_string(int a, int b, int c) {
    std::ostringstream ss;
    ss << a << '.' << b << '.' << c;
    return ss.str();
}

std::unique_ptr<modulog::core::Core> core;

void signalHandler(int signum) {
    std::cout << "Interrupt signal (" << signum << ") received.\n";
    core->stop();
}


int main(int argc, const char **argv) {
    struct sigaction sigAct{};
    memset(&sigAct, 0, sizeof(sigAct));
    sigAct.sa_handler = signalHandler;
    sigaction(SIGINT,  &sigAct, nullptr);
    sigaction(SIGTERM, &sigAct, nullptr);

    auto sharedSettings = std::make_shared<modulog::communication::SharedSettings>();
    std::filesystem::path agentsList = sharedSettings->LogSettings.agentListPath;

    try {
        auto ioContext = std::make_shared<asio::io_context>();
        core = std::make_unique<modulog::core::Core>(std::filesystem::absolute(agentsList), ioContext, sharedSettings);
        core->start();
    } catch (std::exception &e) {
        std::cerr << "Exception in main.cpp: " <<  e.what() << std::endl;
        return EXIT_FAILURE;
    }
    std::cout << "---- END -----" << std::endl;
    return EXIT_SUCCESS;
}
