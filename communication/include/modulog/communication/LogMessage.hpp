#pragma once

#include <modulog/communication/Message.hpp>

namespace modulog::communication{
/**
 * Class is used to transport logs between Core and Agent - it has got type (LOG, ERROR, ...), key and value
 */
    class LogMessage : public Message {
    public:
        /**
         * Message types - in future can be more (like in syslog)
         */
        enum class LOG_MSG_TYPE {
            LOG, ERROR, DEBUG, WARNING
        };

        /**
         * Constructor, if you want to init this class with json (useful when deserializing)
         * @param jsonInit
         */
        LogMessage(std::string jsonInit);

        /**
         * Init this class with all attributes (if you want to create instance from code)
         * @param logMsgType  what is message type (like LOG, ERROR, ...)
         * @param key key of log - must be json key, like "logTemperature"
         * @param value value of log
         */
        LogMessage(LOG_MSG_TYPE logMsgType, std::string key, std::string value);

        /**
         * Used to convert enum class LOG_MSG_TYPE to string - used for log saving, we don't want to have these values as numbers
         * @param logMsgType type to convert to string (like LOG)
         * @return converted string (like "LOG")
         */
        static std::string getTypeStr(LOG_MSG_TYPE logMsgType);

        /**
         * Serialize this message to json format ready to send
         * @return serialized message
         */
        std::string serialize() override;

        /**
         * Get type of this message
         * @return type of message
         */
        LOG_MSG_TYPE getType();

        /**
         * Get key of this message
         * @return key
         */
        std::string getKey();

        /**
         * Get value of this message
         * @return value
         */
        std::string getValue();

    private:
        std::string key_;
        std::string value_;
        LOG_MSG_TYPE logType_;

    };



}
