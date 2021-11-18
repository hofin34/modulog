#include <iostream>
#include "TcpServer.h"


void TcpServer::start_accept()
{
    TcpConnection::pointer new_connection =
            TcpConnection::create(io_context_);

    acceptor_.async_accept(new_connection->get_socket(),
                           std::bind(&TcpServer::handle_accept, this, new_connection,
                                     std::placeholders::_1));
}

void TcpServer::handle_accept(TcpConnection::pointer new_connection,
                   const asio::error_code& error)
{
    if (!error)
    {
        std::cout << "Starting new connection..." << std::endl;
        //new_connection->start_alive_writer();
        new_connection->start_read();
    }
    start_accept();
}