#include <modulog/communication/TcpServer.hpp>

namespace modulog::communication{
    void TcpServer::startAccept()
    {
        auto messageProcessor = std::make_shared<MessageProcessor>(totalReceivedMsgs_, messageConditionVariable_, messageMutex_);
        auto new_connection = std::make_shared<TcpConnection>(io_context_, serverName_, messageProcessor);

        acceptor_.async_accept(new_connection->getSocket(),
                               [this, new_connection](const asio::error_code& e){TcpServer::handleAccept(new_connection, e);});
    }

    void TcpServer::handleAccept(std::shared_ptr<TcpConnection> new_connection,
                                 const asio::error_code& error)
    {
        if (!error)
        {
            std::cout << "Starting new connection..." << std::endl;
            
            lastConnectionsVector.push_back(new_connection);
        }
        startAccept();
    }

    std::shared_ptr<TcpConnection> TcpServer::popConnection() {
        if(lastConnectionsVector.empty())
            return nullptr;
        auto toReturn = lastConnectionsVector.front();
        lastConnectionsVector.erase(lastConnectionsVector.begin());
        return toReturn;
    }

}
