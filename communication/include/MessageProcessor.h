#pragma once

#include <vector>
#include <condition_variable>
#include "LogMessage.h"
#include "ControlMessage.h"
#include "MessageDeserializer.h"
#include "MessageSerializer.h"

class MessageProcessor {
public:
    MessageProcessor(int& messagesToProcess, std::condition_variable &conditionVariable, std::mutex &mutex);
    void processMessage(std::string msg);
    const std::vector<std::shared_ptr<LogMessage>> &getLogMessagesVector() const;
    const std::vector<std::shared_ptr<ControlMessage>> &getControlMessageVector() const;
    std::shared_ptr<ControlMessage> waitForControlMessage();
    std::shared_ptr<LogMessage> waitForLogMessage();
    std::shared_ptr<LogMessage> popLogMessage();
    std::shared_ptr<ControlMessage> popControlMessage();



private:
    std::vector<std::shared_ptr<ControlMessage>> controlMessageVector;
    std::vector<std::shared_ptr<LogMessage>> logMessagesVector;

    std::mutex& mutex_;
    std::condition_variable& conditionVariable_;
    int& messagesToProcess_;
};
