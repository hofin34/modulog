#pragma once

#include <modulog/communication/LogMessage.hpp>
#include <modulog/communication/ControlMessage.hpp>
#include <modulog/communication/Message.hpp>

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
        explicit MessageSerializer(const std::shared_ptr<LogMessage>& logMessage);

        /**
         * Constructor for control message
         * @param controlMessage control message to serialize
         */
        explicit MessageSerializer(const std::shared_ptr<ControlMessage>& controlMessage);

        /**
         * Serialize message passed to constructor and adding envelope for transport marking msg type
         * @return serialized message
         */
        std::string serialize();

    private:
        std::shared_ptr<Message> message_;
        MSG_TYPE messageType_;
    };
}
