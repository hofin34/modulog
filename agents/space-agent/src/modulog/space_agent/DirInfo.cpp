#include <modulog/space_agent/DirInfo.hpp>

namespace modulog::space_agent{
    std::string DirInfo::getId() {
        return folderPath_;
    }

    std::string DirInfo::getTotalSpaceLog() {
        std::string toLog = folderPath_.string() + " : " + std::to_string(getCapacityMiB(folderPath_));
        return toLog;
    }

    std::string DirInfo::getAvailableSpaceLog() {
        std::string toLog = folderPath_.string() + " : " + std::to_string(getAvailableSpaceMiB(folderPath_));
        return toLog;
    }

    float DirInfo::getAvailablePercents() {
        return SpaceInfo::getAvailablePercents(folderPath_);
    }
}
