#include <iostream>
#include <modulog/core/Core.hpp>
#include <vector>
#include <asio.hpp>
#include "meta-lib/include/SharedSettings.hpp"
#include <bringauto/logging/Logger.hpp>
#include <bringauto/logging/ConsoleSink.hpp>

void initLogger(const std::string &logPath, bool verbose) {
    if (verbose) {
        bringauto::logging::Logger::addSink<bringauto::logging::ConsoleSink>();
    }
    /* Probably will have to redirect output, I am not able to set names for each test scenario logs
    bringauto::logging::FileSink::Params paramFileSink{logPath, "log2.txt"};
    paramFileSink.maxFileSize = 1024 * 1024 * 24;
    paramFileSink.numberOfRotatedFiles = 1;
    paramFileSink.verbosity = bringauto::logging::Logger::Verbosity::Debug;
    bringauto::logging::Logger::addSink<bringauto::logging::FileSink>(paramFileSink);
    */
    //bringauto::logging::Logger::addSink<bringauto::logging::FileSink>({logPath, "virtual-vehicle-utility.log"});
    bringauto::logging::Logger::LoggerSettings params{"virtual-vehicle-utility",
                                                      bringauto::logging::Logger::Verbosity::Debug};
    bringauto::logging::Logger::init(params);
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

    initLogger("./ba-logs", true);
    auto sharedSettings = std::make_shared<modulog::meta_lib::SharedSettings>();
    try {
        auto ioContext = std::make_shared<asio::io_context>();
        core = std::make_unique<modulog::core::Core>(ioContext, sharedSettings);
        core->start();
    } catch (std::exception &e) {
        std::cerr << "Exception in main.cpp: " <<  e.what() << std::endl;
        return EXIT_FAILURE;
    }
    std::cout << "---- END -----" << std::endl;
    return EXIT_SUCCESS;
}
