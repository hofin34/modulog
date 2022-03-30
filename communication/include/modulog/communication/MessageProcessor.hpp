#pragma once

#include <modulog/communication/LogMessage.hpp>
#include <modulog/communication/ControlMessage.hpp>
#include <modulog/communication/MessageDeserializer.hpp>
#include <modulog/communication/MessageSerializer.hpp>

#include <vector>
#include <condition_variable>
#include <string>

namespace modulog::communication {
/**
 * Used to handle message receiving - notifying condition variable passed by constructor
 */
    class MessageProcessor {
    public:
        /**
         * @param messagesToProcess counter of total received messages (increased when received, decreased when message popped)
         * @param conditionVariable for syncing - notifying and waiting
         * @param mutex to protect condition variable
         */
        MessageProcessor(int &messagesToProcess, std::condition_variable &conditionVariable, std::mutex &mutex);

        /**
         * Called when message received - it is deserialized and stored in vector, waiting for pop and saving as log
         * @param msg received message to be processed
         */
        void processMessage(std::string msg);

        /**
         * Waits until condition var is not notified or timeout expires
         * @param timeoutMillis how long wait, if passed -1, waits infinitely
         * @return control message or nullptr if timeout expired without receiving message
         */
        std::shared_ptr<ControlMessage> waitForControlMessage(int timeoutMillis);

        /**
         * Pops log message from vector
         * @return log message or nullptr if vector empty
         */
        std::shared_ptr<LogMessage> popLogMessage(); //TODO maybe locking message vectors?

        /**
        * Pops control message from vector
        * @return control message or nullptr if vector empty
        */
        std::shared_ptr<ControlMessage> popControlMessage();


    private:
        std::vector<std::shared_ptr<ControlMessage>> controlMessageVector;
        std::mutex controlMessageVectorMtx_;
        std::vector<std::shared_ptr<LogMessage>> logMessagesVector;
        std::mutex logMessageVectorMtx_;

        std::mutex &mutex_;
        std::condition_variable &conditionVariable_;
        int &messagesToProcess_;
    };

}
