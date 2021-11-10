//
// Created by martin on 10.11.21.
//

#include "LogMessage.h"

std::string LogMessage::serialize() {
    return nlohmann::json{{"type", logType_}, {"value", value_}}.dump();
}
