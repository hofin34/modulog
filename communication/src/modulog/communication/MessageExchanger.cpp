#include <modulog/communication/MessageExchanger.hpp>

namespace modulog::communication{
    MessageExchanger::MessageExchanger(const std::shared_ptr<TcpConnection>& connection) : connection_(connection){
        connection->startRead();
    }

    void MessageExchanger::sendLog(const std::shared_ptr<LogMessage>& logMessage) {
        MessageSerializer messageSerializer(logMessage);
        std::string toSend = messageSerializer.serialize();

        if(connection_)
            connection_->sendMessage(toSend);
        else
            bringauto::logging::Logger::logError("Connection not set, trying to send: {}", toSend);
    }

    void MessageExchanger::sendControl(const std::shared_ptr<ControlMessage> &controlMessage) {
        MessageSerializer messageSerializer(controlMessage);
        std::string toSend = messageSerializer.serialize();
        if(connection_ != nullptr)
            connection_->sendMessage(toSend);
        else
            std::cerr << "Connection not set, simulated send: " << toSend << std::endl;

    }

    std::shared_ptr<ControlMessage> MessageExchanger::waitForControlMessage(int timeoutMillis) {
        return connection_->getMessageProcessor()->waitForControlMessage(timeoutMillis);
    }

    std::shared_ptr<LogMessage> MessageExchanger::popLogMessage() {
        return connection_->getMessageProcessor()->popLogMessage();
    }

    std::shared_ptr<ControlMessage> MessageExchanger::popControlMessage() {
        return connection_->getMessageProcessor()->popControlMessage();
    }

    std::shared_ptr<TcpConnection> MessageExchanger::getConnection() {
        return connection_;
    }


}
