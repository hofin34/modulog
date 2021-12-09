#pragma once
#include "TcpConnection.h"
class MessageExchanger {
public:
    MessageExchanger(TcpConnection::pointer connection);
    void sendLog(const std::shared_ptr<LogMessage>& logMessage);
    void sendControl(const std::shared_ptr<ControlMessage> &controlMessage);
    std::shared_ptr<LogMessage> waitForLogMessage();
    std::shared_ptr<ControlMessage> waitForControlMessage();
    std::shared_ptr<LogMessage> popLogMessage();
    std::shared_ptr<ControlMessage> popControlMessage();

private:
    TcpConnection::pointer connection_;
};
