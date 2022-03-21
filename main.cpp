/*
 * Begin of modulog program. Look at README.md, how to use.
 */
#include <modulog/meta_lib/SharedSettings.hpp>
#include <modulog/core/Core.hpp>

#include <bringauto/logging/Logger.hpp>
#include <bringauto/logging/ConsoleSink.hpp>
#include <bringauto/logging/FileSink.hpp>
#include <asio.hpp>
#include <cxxopts.hpp>

#include <iostream>
#include <vector>

void initLogger(const std::filesystem::path &logPath) {
    bringauto::logging::FileSink::Params paramFileSink{logPath, "one-file-log.txt"};
    paramFileSink.maxFileSize = 150;
    paramFileSink.numberOfRotatedFiles = 2;
    paramFileSink.verbosity = bringauto::logging::Logger::Verbosity::Debug;

    bringauto::logging::Logger::addSink<bringauto::logging::FileSink>(paramFileSink);
    bringauto::logging::Logger::addSink<bringauto::logging::ConsoleSink>();

    bringauto::logging::Logger::LoggerSettings params{"modulog",
                                                      bringauto::logging::Logger::Verbosity::Debug};
    bringauto::logging::Logger::init(params);
}


std::unique_ptr<modulog::core::Core> core;

/**
 * When is modulog interrupted by SIGINT/SIGTERM signal, this function is called - it will stop the core
 * @param error
 * @param signum
 */
void signalHandler(const std::error_code &error,
                   int signum) {
    if (!error) {
        bringauto::logging::Logger::logInfo("Interrupt signal {} received", signum);
        core->stop();
    }
}

/**
 * Function parsing cmd line arguments - in SharedSettings will override default values from cmd line
 * @param argc argument count
 * @param argv pointer to arguments
 * @return returns nullptr if --help argument contained, if error occured, then throwing exception, if parsed successfully, returned shared pointer to SharedSettings
 */
std::shared_ptr<modulog::meta_lib::SharedSettings> parseArgs(std::shared_ptr<modulog::meta_lib::SharedSettings> sharedSettings, int argc, const char **argv) {
    cxxopts::Options options("modulog", "Modular light-weighted logging program");
    options.add_options()
            ("h,help", "Print usage")
            ("e,enabled-agents", "Enabled agents iifile - in this file can be just compiled agents!",
             cxxopts::value<std::string>())
            ("o,one-file", "All logs will be merged in one file", cxxopts::value<bool>());
    auto result = options.parse(argc, argv);
    if (result.count("help")) {
        std::cout << options.help() << std::endl;
        return nullptr;
    }
    if (result.count("enabled-agents"))
        sharedSettings->LogSettings.enabledAgentsPath = result["enabled-agents"].as<std::string>();
    if(result.count("one-file"))
        sharedSettings->LogSettings.oneFileLog = true;

    return sharedSettings;
}

int main(int argc, const char **argv) { //TODO --one-file not logging into file!
    auto sharedSettings = std::make_shared<modulog::meta_lib::SharedSettings>();
    initLogger(sharedSettings->LogSettings.logsDestination);
    try {
#ifdef BRINGAUTO_TESTS
        sharedSettings->Testing.initTesting();
#endif
        sharedSettings = parseArgs(sharedSettings, argc, argv);
        if (!sharedSettings) // if --help arg
            return EXIT_SUCCESS;
        auto ioContext = std::make_shared<asio::io_context>();
        asio::signal_set signals(*ioContext, SIGINT, SIGTERM);
        signals.async_wait(signalHandler);
        core = std::make_unique<modulog::core::Core>(ioContext, sharedSettings);
        core->start();
    } catch (std::exception &e) {
        bringauto::logging::Logger::logError("Exception in main.cpp: {}", e.what());
        core.reset();
        return EXIT_FAILURE;
    }
    core.reset();
    bringauto::logging::Logger::logInfo("---- END ----");
    return EXIT_SUCCESS;
}
