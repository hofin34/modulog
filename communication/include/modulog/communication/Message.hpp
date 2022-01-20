#pragma once

#include <string>
#include <nlohmann/json.hpp>
#include <iomanip>
#include <iostream>

namespace modulog::communication {

/**
 * Parent class of LogMessage and ControlMessage - used for communication between Agent and Core
 */
    class Message {
    public:
        /**
         * Two types of messages - one for sending logs and second for controling actions like checking if is agent
         * alive and then responds to it etc.
         */
        enum class MSG_TYPE {
            LOG_MSG, CONTROL_MSG
        };

        /**
         * Serialize message - log message and control message is implemented differently (they have got different attributes)
         * @return serialized message
         */
        virtual std::string serialize() = 0;

        /**
         * Message contains attribute timestamp - default is not defined, but when is received, it is set (to log it later)
         */
        void setTimestampNow();

        /**
         * get timestamp
         * @return timestamp string
         */
        std::string getTimestamp();

    private:
        std::string timestamp_ = "notDefinedTimestamp";
    };

}
