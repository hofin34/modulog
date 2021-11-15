#include <iostream>
#include "TcpServer.h"
#include <vector>
#include <asio.hpp>


int main(int argc, const char **argv) {
    try
    {
        asio::io_context io_context;
        TcpServer server(io_context);
        server.start_accept();
        //std::thread thread1{[&io_context](){ io_context.run(); }};
        //std::thread thread2{[&io_context](){ io_context.run(); }};
        //thread1.join();
        //thread2.join();

        io_context.run();
    }
    catch (std::exception& e)
    {
        std::cerr << e.what() << std::endl;
    }

    return 0;

}
