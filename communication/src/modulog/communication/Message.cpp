#include <modulog/communication/Message.hpp>

namespace modulog::communication{
    void Message::setTimestampNow() {
        timestamp_ = meta_lib::Helpers::getTimestamp();
    }

    std::string Message::getTimestamp() {
        return timestamp_;
    }
}

