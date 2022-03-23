#pragma once

#include <filesystem>

namespace modulog::space_agent {
    class SpaceInfo {
    public:
        /**
         *
         * @return string containing total space info to be logged (will be different for partition and for standalone folder)
         */
        virtual std::string getTotalSpaceLog() = 0;

        /**
         *
         * @return string containing available space info to be logged
         */
        virtual std::string getAvailableSpaceLog() = 0;

        /**
         *
         * @return how many percents is available in space
         */
        virtual float getAvailablePercents() = 0;

        /**
         *
         * @return id of space unit - if folder, than it is path to folder, if partition, than it is partition path like /dev/sda (not mounted path)
         */
        virtual std::string getId() = 0;

    protected:
        /**
         * Helper function to calculate available space from childs
         * @param path
         * @return
         */
        static int getAvailableSpaceMiB(const std::filesystem::path& path);

        /**
         * Helper function to calculate capacity from child
         * @param path
         * @return
         */
        static int getCapacityMiB(const std::filesystem::path& path);

        /**
         * Helper function to calculate available percents from child
         * @param path
         * @return
         */
        static float getAvailablePercents(const std::filesystem::path& path);


    };
}

