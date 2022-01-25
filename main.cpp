#include <iostream>
#include <modulog/communication/TcpServer.hpp>
#include <modulog/core/Core.hpp>
#include <vector>
#include <asio.hpp>

// Just for debug, TODO remove later:
std::string ver_string(int a, int b, int c) {
    std::ostringstream ss;
    ss << a << '.' << b << '.' << c;
    return ss.str();
}

std::string true_cxx =
#ifdef __clang__
        "clang++";
#else
        "g++";
#endif

std::string true_cxx_ver =
#ifdef __clang__
        ver_string(__clang_major__, __clang_minor__, __clang_patchlevel__);
#else
        ver_string(__GNUC__, __GNUC_MINOR__, __GNUC_PATCHLEVEL__);
#endif


std::unique_ptr<modulog::core::Core> core;

void signalHandler( int signum ) {
    std::cout << "Interrupt signal (" << signum << ") received.\n";
    core->stop();
    exit(signum);
}


int main(int argc, const char **argv) {
    std::cout << "Comp. version: " << true_cxx_ver << std::endl;
    try{
        signal(SIGINT, signalHandler);
        signal(SIGTERM, signalHandler);

        std::filesystem::path agentsList = "../agents-list.conf"; // TODO MOVE to config file
        auto ioContext = std::make_shared<asio::io_context>();
        core = std::make_unique<modulog::core::Core>(std::filesystem::absolute(agentsList), ioContext);
        core->start();
    }catch(std::exception& e){
        std::cerr << e.what() << std::endl;
        return EXIT_FAILURE;
    }

    return 0;
}
