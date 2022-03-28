#include <modulog/space_agent/SpaceInfo.hpp>

namespace modulog::space_agent{
    int SpaceInfo::getAvailableSpaceMiB(const std::filesystem::path& path) {
        auto spaceInfo = std::filesystem::space(path);
        return spaceInfo.available / BYTES_IN_MIB;
    }

    int SpaceInfo::getCapacityMiB(const std::filesystem::path& path) {
        auto spaceInfo = std::filesystem::space(path);
        return spaceInfo.capacity / BYTES_IN_MIB;
    }

    float SpaceInfo::getAvailablePercents(const std::filesystem::path& path) {
        return (float) getAvailableSpaceMiB(path) / getCapacityMiB(path) * 100;
    }


}
