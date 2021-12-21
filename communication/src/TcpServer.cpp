#include <iostream>
#include "../include/TcpServer.h"


void TcpServer::start_accept()
{
    auto messageProcessor = std::make_shared<MessageProcessor>(totalReceivedMsgs_, messageConditionVariable_, messageMutex_);
    TcpConnection::pointer new_connection =
            TcpConnection::create(io_context_, serverName_, messageProcessor);

    acceptor_.async_accept(new_connection->getSocket(),
                           std::bind(&TcpServer::handle_accept, this, new_connection,
                                     std::placeholders::_1));
}

void TcpServer::handle_accept(TcpConnection::pointer new_connection,
                   const asio::error_code& error)
{
    if (!error)
    {
        std::cout << "Starting new connection..." << std::endl;
        lastConnectionsVector.push_back(new_connection);
    }
    start_accept();
}

TcpConnection::pointer TcpServer::popConnection() {
    if(lastConnectionsVector.empty())
        return nullptr;
    auto toReturn = lastConnectionsVector.front();
    lastConnectionsVector.erase(lastConnectionsVector.begin());
    return toReturn;
}
