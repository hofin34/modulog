#include <iostream>
#include "communication/include/TcpServer.h"
#include "core/include/Core.h"
#include <vector>
#include <asio.hpp>


int main(int argc, const char **argv) {
    try{
        std::filesystem::path agentsList = "../agents-list.conf";
        auto ioContext = std::make_shared<asio::io_context>();
        Core core(std::filesystem::absolute(agentsList), ioContext);
        core.start();
    }catch(std::exception& e){
        std::cerr << e.what() << std::endl;
        return EXIT_FAILURE;
    }

    return 0;
}
