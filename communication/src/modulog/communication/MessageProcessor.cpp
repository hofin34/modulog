#include <modulog/communication/MessageProcessor.hpp>

namespace modulog::communication{
    MessageProcessor::MessageProcessor(int &messagesToProcess, std::condition_variable &conditionVariable,
                                       std::mutex &mutex) : messagesToProcess_(messagesToProcess),
                                                            conditionVariable_(conditionVariable),
                                                            mutex_(mutex) {

    }

    void MessageProcessor::processMessage(std::string msg) {
        {
            std::lock_guard<std::mutex> lck(mutex_);
            messagesToProcess_++;
        }
        MessageDeserializer messageDeserializer(msg);
        if (messageDeserializer.getMsgType() == MSG_TYPE::CONTROL_MSG) {
            std::lock_guard<std::mutex> lck(controlMessageVectorMtx_);
            controlMessageVector.push_back(messageDeserializer.getControlMessage());
        } else if (messageDeserializer.getMsgType() == MSG_TYPE::LOG_MSG) {
            std::lock_guard<std::mutex> lck(logMessageVectorMtx_);
            logMessagesVector.push_back(messageDeserializer.getLogMessage());
        }
        conditionVariable_.notify_one();
    }


    std::shared_ptr<ControlMessage> MessageProcessor::waitForControlMessage(int timeoutMillis) {
        auto endTime = std::chrono::system_clock::now() + std::chrono::milliseconds(timeoutMillis);
        if (timeoutMillis == -1)
            endTime = std::chrono::system_clock::now() + std::chrono::seconds(42);
        while (std::chrono::system_clock::now() <= endTime) {
            if (timeoutMillis == -1) { // infinite wait branch
                std::unique_lock<std::mutex> lck(mutex_);
                conditionVariable_.wait(lck, [this] { return messagesToProcess_; });
                if (!controlMessageVector.empty()) {
                    messagesToProcess_--;
                    return popControlMessage();
                } else { // notified by log message (or control from another agent) - continue waiting
                    endTime = std::chrono::system_clock::now() +
                              std::chrono::seconds(42); // making endless while by creating always true condition.
                    continue;
                }
            } else {
                std::unique_lock<std::mutex> lck(mutex_);
                if (conditionVariable_.wait_for(lck, std::chrono::milliseconds(timeoutMillis),
                                                [this] { return messagesToProcess_; })) {
                    // conditionVar was notified, but it could be notified by log message
                    if (!controlMessageVector.empty()) {
                        messagesToProcess_--;
                        return popControlMessage();
                    }
                }
            }

        }
        return nullptr; // timeout
    }


    std::shared_ptr<LogMessage> MessageProcessor::popLogMessage() {
        std::lock_guard<std::mutex> lck(logMessageVectorMtx_);
        if (logMessagesVector.empty())
            return nullptr;
        auto msgToReturn = logMessagesVector.front();
        logMessagesVector.erase(logMessagesVector.begin());
        return msgToReturn;
    }

    std::shared_ptr<ControlMessage> MessageProcessor::popControlMessage() {
        std::lock_guard<std::mutex> lck(controlMessageVectorMtx_);
        if (controlMessageVector.empty())
            return nullptr;
        auto msgToReturn = controlMessageVector.front();
        controlMessageVector.erase(controlMessageVector.begin());
        return msgToReturn;
    }

}
