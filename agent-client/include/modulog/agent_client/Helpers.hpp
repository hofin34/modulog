#pragma once

#include <nlohmann/json.hpp>
#include <bringauto/logging/ConsoleSink.hpp>

#include <filesystem>
#include <fstream>
#include <iostream>


namespace modulog::agent_client {
    /**
     * Class containing helper functions for repeatig routines of Agents
     */
    class Helpers {
    public:
        /**
         * Runs system command and gets its output
         * @param cmd command to be run (like "cat /path/to/file.txt"
         * @return output of command
         */
        static std::string execCommand(const std::string &cmd);

        /**
         * Parse agents config - config file named "config.json" is saved in same directory as agent executable
         * This functions locate this file, reads its json content and parses it
         * @param execPath Path to executable
         * @return parsed nlohmann::json object
         */
        static nlohmann::json parseConfig(const std::filesystem::path &execPath);

    };

}


