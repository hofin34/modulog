#include <modulog/watchdog_agent/UdpServer.h>
namespace modulog::watchdog_agent{
    UdpServer::UdpServer(asio::io_service &ioContext, int port, std::shared_ptr<WatchdogHandler> watchdogHandler) : socket_(ioContext,
                                                                                                                            asio::ip::udp::endpoint(asio::ip::udp::v4(),
                                                                                                   port)),
                                                                                                   watchdogHandler_(watchdogHandler){
        startReceive();
    }

    void UdpServer::startReceive() {
        socket_.async_receive_from(
                asio::buffer(recvBuffer_), remoteEndpoint_,
                [this](const asio::error_code &errorCode, std::size_t bytesTransfered) {
                    handleReceive(errorCode, bytesTransfered);
                });
    }

    void UdpServer::handleReceive(const asio::error_code &error,
                                  std::size_t /*bytes_transferred*/) {
        if (!error || error == asio::error::message_size) {
            watchdogHandler_->processMessage(recvBuffer_.data());
            recvBuffer_.fill('\0');
            startReceive();
        }
    }
}
