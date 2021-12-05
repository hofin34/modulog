#include <string>
#include "MessageProcessor.h"
#include "MessageDeserializer.h"

void MessageProcessor::processMessage(std::string msg) {
    {
        std::lock_guard<std::mutex> lck(mutex_);
        messagesToProcess_++;
    }
    std::cout << "Processing msg: " << msg << std::endl;
    MessageDeserializer messageDeserializer(msg);
    if(messageDeserializer.getMsgType() == Message::MSG_TYPE::CONTROL_MSG){
        controlMessageVector.push_back(messageDeserializer.getControlMessage());
    }else if(messageDeserializer.getMsgType() == Message::MSG_TYPE::LOG_MSG){
        logMessagesVector.push_back(messageDeserializer.getLogMessage());
    }
    conditionVariable_.notify_one();
}

MessageProcessor::MessageProcessor(int &messagesToProcess, std::condition_variable &conditionVariable,
                                   std::mutex &mutex) : messagesToProcess_(messagesToProcess), conditionVariable_(conditionVariable),
                                   mutex_(mutex){

}

const std::vector<std::shared_ptr<LogMessage>> &MessageProcessor::getLogMessagesVector() const {
    return logMessagesVector;
}

const std::vector<std::shared_ptr<ControlMessage>> &MessageProcessor::getControlMessageVector() const {
    return controlMessageVector;
}

std::shared_ptr<ControlMessage> MessageProcessor::waitForControlMessage() {

    std::unique_lock<std::mutex> lck(mutex_);
    conditionVariable_.wait(lck, [this]{ return messagesToProcess_; });
    if(controlMessageVector.empty())
        return nullptr;
    messagesToProcess_--;
    return popControlMessage();
}

std::shared_ptr<LogMessage> MessageProcessor::waitForLogMessage() {
    std::unique_lock<std::mutex> lck(mutex_);
    conditionVariable_.wait(lck, [this]{ return messagesToProcess_; });
    if(logMessagesVector.empty())
        return nullptr;
    messagesToProcess_--;
    return popLogMessage();
}

std::shared_ptr<LogMessage> MessageProcessor::popLogMessage() {
    if(logMessagesVector.empty())
        return nullptr;
    auto msgToReturn = logMessagesVector.front();
    logMessagesVector.erase(logMessagesVector.begin());
    return msgToReturn;
}

std::shared_ptr<ControlMessage> MessageProcessor::popControlMessage() {
    if(controlMessageVector.empty())
        return nullptr;
    auto msgToReturn = controlMessageVector.front();
    controlMessageVector.erase(controlMessageVector.begin());
    return msgToReturn;}
