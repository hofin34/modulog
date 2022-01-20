#pragma once

#include "LogMessage.hpp"
#include "ControlMessage.hpp"
#include "Message.hpp"

namespace modulog::communication{
    /**
 * Takes care of serializing messages - takes concrete message (Log or Control) and gives it in envelope marking its type
 */
    class MessageSerializer {
    public:
        /**
         * Constructor for log message
         * @param logMessage log message to serialize
         */
        MessageSerializer(std::shared_ptr<LogMessage> logMessage);

        /**
         * Constructor for control message
         * @param controlMessage control message to serialize
         */
        MessageSerializer(std::shared_ptr<ControlMessage> controlMessage);

        /**
         * Serialize message passed to constructor and adding envelope for transport marking msg type
         * @return serialized message
         */
        std::string serialize();

    private:
        std::shared_ptr<Message> message_;
        Message::MSG_TYPE messageType_;
    };
}
