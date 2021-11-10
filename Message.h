#pragma once

#include <string>
#include <nlohmann/json.hpp>



class Message {
private:
    std::string timestamp;
    virtual std::string serialize() = 0;
};
