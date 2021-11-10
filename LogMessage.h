#pragma once
#include "Message.h"
class LogMessage : Message{
public:
    LogMessage deserialize(std::string toDeserialize);
    std::string serialize() override;
private:
    enum type{LOG, ERROR, DEBUG};
    std::string value_;
    type logType_;

};


