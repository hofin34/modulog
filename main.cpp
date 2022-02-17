/*
 * Begin of modulog program. Look at README.md, how to use.
 */
#include <modulog/meta_lib/SharedSettings.hpp>
#include <modulog/core/Core.hpp>

#include <bringauto/logging/Logger.hpp>
#include <bringauto/logging/ConsoleSink.hpp>
#include <asio.hpp>
#include <cxxopts.hpp>

#include <iostream>
#include <vector>

void initLogger(const std::string &logPath, bool verbose) {
    if (verbose) {
        bringauto::logging::Logger::addSink<bringauto::logging::ConsoleSink>();
    }
    bringauto::logging::Logger::LoggerSettings params{"modulog",
                                                      bringauto::logging::Logger::Verbosity::Debug};
    bringauto::logging::Logger::init(params);
}


std::unique_ptr<modulog::core::Core> core;

void signalHandler(int signum) {
    bringauto::logging::Logger::logInfo("Interrupt signal {} received", signum);
    core->stop();
}


int main(int argc, const char **argv) {
    initLogger("./ba-logs", true);
    auto sharedSettings = std::make_shared<modulog::meta_lib::SharedSettings>();
    cxxopts::Options options("modulog", "Modular light-weighted logging program");
    options.add_options()
            ("h,help", "Print usage")
            ("e,enabled-agents", "Enabled agents file - in this file can be just compiled agents!", cxxopts::value<std::string>())
            ;
    auto result = options.parse(argc, argv);
    if (result.count("help")){
        std::cout << options.help() << std::endl;
        exit(EXIT_SUCCESS);
    }
    if(result.count("enabled-agents"))
        sharedSettings->LogSettings.enabledAgentsPath = result["bar"].as<std::string>();

    struct sigaction sigAct{};
    memset(&sigAct, 0, sizeof(sigAct));
    sigAct.sa_handler = signalHandler;
    sigaction(SIGINT, &sigAct, nullptr);
    sigaction(SIGTERM, &sigAct, nullptr);

    try {
        auto ioContext = std::make_shared<asio::io_context>();
        core = std::make_unique<modulog::core::Core>(ioContext, sharedSettings);
        core->start();
    } catch (std::exception &e) {
        bringauto::logging::Logger::logError("Exception in main.cpp: {}", e.what());
        return EXIT_FAILURE;
    }
    bringauto::logging::Logger::logInfo("---- END ----");
    return EXIT_SUCCESS;
}
