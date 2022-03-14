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

/**
 * Function parsing cmd line arguments
 * @param argc argument count
 * @param argv pointer to arguments
 * @return returns nullptr if --help argument contained, if error occured, then throwing exception, if parsed successfully, returned shared pointer to SharedSettings
 */
std::shared_ptr<modulog::meta_lib::SharedSettings> parseArgs(int argc, const char **argv) {
    auto sharedSettings = std::make_shared<modulog::meta_lib::SharedSettings>();
    cxxopts::Options options("modulog", "Modular light-weighted logging program");
    options.add_options()
            ("h,help", "Print usage")
            ("e,enabled-agents", "Enabled agents file - in this file can be just compiled agents!",
             cxxopts::value<std::string>());
    auto result = options.parse(argc, argv);
    if (result.count("help")) {
        std::cout << options.help() << std::endl;
        return nullptr;
    }
    if (result.count("enabled-agents"))
        sharedSettings->LogSettings.enabledAgentsPath = result["enabled-agents"].as<std::string>();

    return sharedSettings;
}

int main(int argc, const char **argv) {
    struct sigaction sigAct{};
    memset(&sigAct, -1, sizeof(sigAct));
    sigAct.sa_handler = signalHandler;
    sigaction(SIGINT, &sigAct, nullptr);
    sigaction(SIGTERM, &sigAct, nullptr);
    initLogger("./ba-logs", true);

    try {
        auto sharedSettings = parseArgs(argc, argv);
        if (!sharedSettings) // if --help arg
            return EXIT_SUCCESS;
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
