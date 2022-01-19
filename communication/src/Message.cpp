//
// Created by martin on 10.11.21.
//

#include "../include/Message.hpp"
void Message::setTimestampNow() {
    auto t = std::time(nullptr);
    auto tm = *std::localtime(&t);
    std::ostringstream oss;
    oss << std::put_time(&tm, "%Y-%m-%dT%H:%M:%S");
    timestamp_ = oss.str();
}

std::string Message::getTimestamp() {
    return timestamp_;
}
