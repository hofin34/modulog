#include <iostream>
#include "TcpServer.h"
#include "Core.h"
#include <vector>
#include <asio.hpp>


int main(int argc, const char **argv) {
    try{
        std::filesystem::path agentsConfigPath = "../agents-configs/";
        auto ioContext = std::make_shared<asio::io_context>();
        Core core(std::filesystem::absolute(agentsConfigPath), ioContext);
        core.start();
    }catch(std::exception& e){
        std::cerr << e.what() << std::endl;
        return EXIT_FAILURE;
    }


    return 0;

}
