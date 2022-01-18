#include <iostream>
#include "../include/TcpServer.h"


void TcpServer::startAccept()
{
    auto messageProcessor = std::make_shared<MessageProcessor>(totalReceivedMsgs_, messageConditionVariable_, messageMutex_);
    TcpConnection::pointer new_connection =
            TcpConnection::create(io_context_, serverName_, messageProcessor);

    acceptor_.async_accept(new_connection->getSocket(),
                           std::bind(&TcpServer::handleAccept, this, new_connection,
                                     std::placeholders::_1));
}

void TcpServer::handleAccept(TcpConnection::pointer new_connection,
                   const asio::error_code& error)
{
    if (!error)
    {
        std::cout << "Starting new connection..." << std::endl;
        lastConnectionsVector.push_back(new_connection);
    }
    startAccept();
}

TcpConnection::pointer TcpServer::popConnection() {
    if(lastConnectionsVector.empty())
        return nullptr;
    auto toReturn = lastConnectionsVector.front();
    lastConnectionsVector.erase(lastConnectionsVector.begin());
    return toReturn;
}
