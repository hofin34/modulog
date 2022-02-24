#include <modulog/meta_lib/Helpers.h>

namespace modulog::meta_lib{
    std::string Helpers::getTimestamp() {
        auto t = std::time(nullptr);
        auto tm = *std::localtime(&t);
        std::ostringstream oss;
        oss << std::put_time(&tm, "%Y-%m-%dT%H:%M:%S");
        return oss.str();
    }
}
