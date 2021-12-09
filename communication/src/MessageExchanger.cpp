#include "MessageExchanger.h"

MessageExchanger::MessageExchanger(TcpConnection::pointer connection) : connection_(connection){
    connection->start_read();
}

void MessageExchanger::sendLog(const std::shared_ptr<LogMessage>& logMessage) {
    MessageSerializer messageSerializer(logMessage);
    std::string toSend = messageSerializer.serialize();

    if(connection_ != nullptr)
        connection_->send_message(toSend);
    else
        std::cerr << "Connection not set, simulated send: " << toSend << std::endl;


}

void MessageExchanger::sendControl(const std::shared_ptr<ControlMessage> &controlMessage) {
    MessageSerializer messageSerializer(controlMessage);
    std::string toSend = messageSerializer.serialize();
    if(connection_ != nullptr)
        connection_->send_message(toSend);
    else
        std::cerr << "Connection not set, simulated send: " << toSend << std::endl;

}

std::shared_ptr<ControlMessage> MessageExchanger::waitForControlMessage() {
    return connection_->getMessageProcessor_()->waitForControlMessage();
}

std::shared_ptr<LogMessage> MessageExchanger::waitForLogMessage() {
    return connection_->getMessageProcessor_()->waitForLogMessage();
}

std::shared_ptr<LogMessage> MessageExchanger::popLogMessage() {
    return connection_->getMessageProcessor_()->popLogMessage();
}

std::shared_ptr<ControlMessage> MessageExchanger::popControlMessage() {
    return connection_->getMessageProcessor_()->popControlMessage();
}

