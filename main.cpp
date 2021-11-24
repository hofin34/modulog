#include <iostream>
#include "TcpServer.h"
#include "Core.h"
#include <vector>
#include <asio.hpp>


int main(int argc, const char **argv) {
    try{
        std::filesystem::path agentsConfigPath = "../agents-configs/";
        Core core(std::filesystem::absolute(agentsConfigPath));
        core.start();
    }catch(std::exception& e){
        std::cerr << e.what() << std::endl;
        return EXIT_FAILURE;
    }


    return 0;

}
