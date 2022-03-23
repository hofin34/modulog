#pragma once

#include <modulog/space_agent/SpaceInfo.hpp>
#include <filesystem>
#include <utility>

namespace modulog::space_agent {
    class MountInfo : public SpaceInfo {
    public:
        MountInfo(std::filesystem::path partitionPath, std::filesystem::path mountPath) : partitionPath_(std::move(partitionPath)),
                                                                                          mountPath_(std::move(mountPath)) {};

        std::string getId() override;

        std::string getTotalSpaceLog() override;

        std::string getAvailableSpaceLog() override;

        float getAvailablePercents() override;


    private:
        std::filesystem::path partitionPath_;
        std::filesystem::path mountPath_;
    };
}

