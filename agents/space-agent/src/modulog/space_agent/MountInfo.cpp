#include <modulog/space_agent/MountInfo.hpp>

namespace modulog::space_agent{

    std::string MountInfo::getId() {
        return partitionPath_;
    }

    std::string MountInfo::getTotalSpaceLog() {
        std::string toLog = partitionPath_.string() + " : " + mountPath_.string() + " : " +
                            std::to_string(getCapacityMiB(mountPath_));
        return toLog;
    }

    std::string MountInfo::getAvailableSpaceLog() {
        std::string toLog = partitionPath_.string() + " : " + mountPath_.string() + " : " +
                            std::to_string(getAvailableSpaceMiB(mountPath_));
        return toLog;
    }

    float MountInfo::getAvailablePercents() {
        return SpaceInfo::getAvailablePercents(mountPath_);
    }

}
