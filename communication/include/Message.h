#pragma once

#include <string>
#include <nlohmann/json.hpp>
#include <iomanip>
#include <iostream>

class Message {
public:
    enum class MSG_TYPE{LOG_MSG, CONTROL_MSG};
    virtual std::string serialize() = 0;
    void setTimestampNow();
    std::string getTimestamp();
private:
    std::string timestamp_ = "notDefinedTimestamp";
};
