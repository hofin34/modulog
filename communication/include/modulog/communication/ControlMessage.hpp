#pragma once
#include <modulog/communication/Message.hpp>

#include <string>

namespace modulog::communication {
    /**
  * Class representing control message - is used to communicate between Core and Agents
  * Contains information like key-value
  */
    class ControlMessage : public Message {
    public:
        enum class CONTROL_MSG_TYPE {
            CONFIG, ACK, IS_ALIVE, EXIT, EXIT_ERR, EXIT_ACK
        };

        ControlMessage(std::string jsonInit);

        ControlMessage(CONTROL_MSG_TYPE msgType, std::string value);

        /**
         * @return type of control message (IS_ALIVE, ACK, ...)
         */
        CONTROL_MSG_TYPE getType();

        /**
         * Gets value of message
         * @return value
         */
        std::string getValue();

        /**
         * Serialize message for transport
         * @return serialized message
         */
        std::string serialize() override;

    private:
        CONTROL_MSG_TYPE type_;
        std::string value_;
    };

}

