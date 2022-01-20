#pragma once
#include <modulog/communication/ControlMessage.hpp>
#include <modulog/communication/LogMessage.hpp>

#include <nlohmann/json.hpp>

#include <string>
#include <memory>
#include <iostream>



namespace modulog::communication{
/**
 * If you have got message represented by string, you can use this class to deserialize it.
 * After calling constructor, there will be stored log or control message in this class -
 * you have to check it with getMsgType and then getControlMessage() or getLogMessage()
 */
    class MessageDeserializer {
    public:
        /**
         * Parse string and init this class
         * @param toDeserialize message represented by string (usually after transport)
         */
        MessageDeserializer(const std::string &toDeserialize);

        /**
         * Checks if it is Control or Log message
         * @return message type
         */
        Message::MSG_TYPE getMsgType();

        /**
         * Get control message - it can be null! (if was deserialized log message)
         * @return ControlMessage or null
         */
        std::shared_ptr<ControlMessage> getControlMessage();

        /**
         * Get log message - it can be null!
         * @return LogMessage or null
         */
        std::shared_ptr<LogMessage> getLogMessage();

    private:
        Message::MSG_TYPE msgType_;
        std::shared_ptr<ControlMessage> controlMessage_;
        std::shared_ptr<LogMessage> logMessage_;
    };

}
