#include <iostream>
#include "TcpServer.h"
#include "Core.h"
#include <vector>
#include <asio.hpp>


int main(int argc, const char **argv) {
    try{
        Core core;
        core.start();
    }catch(std::exception& e){
        //std::cerr << e.what() << std::endl;
    }


    return 0;

}
