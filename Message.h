#pragma once

#include <string>
#include <nlohmann/json.hpp>


class Message {
public:
    enum class MSG_TYPE{LOG_MSG, CONTROL_MSG};
    virtual std::string serialize() = 0;

private:
    std::string timestamp;
};
