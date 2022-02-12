#include <modulog/communication/Message.hpp>

namespace modulog::communication{
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
}

