#include <modulog/space_agent/SpaceInfo.hpp>

namespace modulog::space_agent{
    int SpaceInfo::getAvailableSpaceMiB(const std::filesystem::path& path) {
        auto spaceInfo = std::filesystem::space(path);
        return spaceInfo.available / (1024 * 1024);
    }

    int SpaceInfo::getCapacityMiB(const std::filesystem::path& path) {
        auto spaceInfo = std::filesystem::space(path);
        return spaceInfo.capacity / (1024 * 1024);
    }

    float SpaceInfo::getAvailablePercents(const std::filesystem::path& path) {
        return (float) getAvailableSpaceMiB(path) / getCapacityMiB(path) * 100;
    }


}
