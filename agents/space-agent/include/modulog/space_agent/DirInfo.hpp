#pragma once

#include <modulog/space_agent/SpaceInfo.hpp>
#include <filesystem>
#include <utility>

namespace modulog::space_agent {
    class DirInfo : public SpaceInfo {
    public:
        explicit DirInfo(std::filesystem::path folderPath) : folderPath_(std::move(folderPath)) {};

        std::string getId() override;

        std::string getTotalSpaceLog() override;

        std::string getAvailableSpaceLog() override;

        float getAvailablePercents() override;

    private:
        std::filesystem::path folderPath_;
    };
}

