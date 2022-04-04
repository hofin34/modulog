#pragma once

#include <modulog/communication/TcpConnection.hpp>

namespace modulog::communication {
/**
 * Class used to send and receive logs between Agent and Core
 */
    class MessageExchanger {
    public:
        /**
         * @param connection active connection with someone
         */
        explicit MessageExchanger(const std::shared_ptr<TcpConnection> &connection);

        /**
         * send log to the second side of connection
         * @param logMessage message to send
         */
        void sendLog(const std::shared_ptr<LogMessage> &logMessage);

        /**
         * send control message to the second side of connection
         * @param controlMessage message to send
         */
        void sendControl(const std::shared_ptr<ControlMessage> &controlMessage);

        /**
         * Condition variable wait for receiving control message
         * @return received log message
         */
        std::shared_ptr<ControlMessage> waitForControlMessage(int timeoutMillis);

        /**
         * Pops log message
         * @return log message or null if not received log message
         */
        std::shared_ptr<LogMessage> popLogMessage();

        /**
         * Pops control message
         * @return control message or null if not received control message
         */
        std::shared_ptr<ControlMessage> popControlMessage();

        std::shared_ptr<TcpConnection> getConnection();

        static constexpr int INFINITE_TIMEOUT = -1;

    private:
        std::shared_ptr<TcpConnection> connection_ = nullptr;
    };

}
