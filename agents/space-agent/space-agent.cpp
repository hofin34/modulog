/*
 * modulog agent, that monitors disk space (look at README.md for more info)
 */

#include <modulog/agent_client/AgentClient.hpp>
#include <modulog/agent_client/ClientFactory.hpp>
#include <modulog/agent_client/Helpers.hpp>
#include <modulog/space_agent/SpaceInfo.hpp>
#include <modulog/space_agent/DirInfo.hpp>
#include <modulog/space_agent/MountInfo.hpp>

#include <thread>
#include <filesystem>
#include <iostream>
#include <mntent.h>




std::vector<std::shared_ptr<modulog::space_agent::SpaceInfo>> getDirsInfo(const std::vector<std::filesystem::path> &foldersVector) {
    std::vector<std::shared_ptr<modulog::space_agent::SpaceInfo>> mountInfoVec;
    for (auto &folder: foldersVector) {
        auto dirInfo = std::make_shared<modulog::space_agent::DirInfo>(folder);
        mountInfoVec.push_back(dirInfo);
    }
    return mountInfoVec;
}

std::vector<std::shared_ptr<modulog::space_agent::SpaceInfo>>
getMountedDirs(const char *filename, const std::vector<std::filesystem::path> &devicesVector,
               const std::shared_ptr<modulog::agent_client::AgentClient> &agentClient) {
    std::vector<std::shared_ptr<modulog::space_agent::SpaceInfo>> mountInfoVec;
    FILE *fp;
    struct mntent *fs;

    fp = setmntent(filename, "r");
    if (!fp) {
        auto errMsg = std::make_shared<modulog::communication::LogMessage>(
                modulog::communication::LogMessage::LOG_MSG_TYPE::ERROR, "errors", "setmntent function call error");
        agentClient->sendLog(errMsg);
        return mountInfoVec;
    }

    for (auto &partition: devicesVector) {
        bool partitionMounted = false;
        while ((fs = getmntent(fp)) != nullptr) { // go through all mounted devices
            if (partition == fs->mnt_fsname) {
                auto mountInfo = std::make_shared<modulog::space_agent::MountInfo>(partition, fs->mnt_dir);
                mountInfoVec.push_back(mountInfo);
                partitionMounted = true;
                break;
            }
        }
        if (!partitionMounted) {
            auto errMsg = std::make_shared<modulog::communication::LogMessage>(
                    modulog::communication::LogMessage::LOG_MSG_TYPE::ERROR, "errors",
                    partition.string() + " not mounted.");
            agentClient->sendLog(errMsg);
        }
    }

    endmntent(fp);
    return mountInfoVec;
}


int main(int argc, char **argv) {

    nlohmann::json configJson = modulog::agent_client::Helpers::parseConfig(argv[0]);
    if (!configJson.contains("id")) {
        std::cerr << "You must specify id!" << std::endl;
        return EXIT_FAILURE;
    }
    std::vector<std::filesystem::path> partitions;
    if (configJson.contains("partitionsToMonitor")) {
        partitions = configJson["partitionsToMonitor"].get<std::vector<std::filesystem::path>>();
    }


    std::vector<std::filesystem::path> folders;
    if (configJson.contains("foldersToMonitor")) {
        folders = configJson["foldersToMonitor"].get<std::vector<std::filesystem::path>>();
    }
    int logInterval = 10;
    if (configJson.contains("logInterval"))
        logInterval = configJson["logInterval"];
    float percentTreshold = 0;
    if (configJson.contains("availableNotSmallerThanPercent"))
        percentTreshold = configJson["availableNotSmallerThanPercent"];
    auto ioContext = std::make_shared<asio::io_context>();
    auto agentClient = modulog::agent_client::ClientFactory::createClient(ioContext, configJson["id"]);
    agentClient->initClient();

    std::string file = "/etc/mtab";
    auto mountInfoVector = getMountedDirs(file.c_str(), partitions, agentClient);
    if(mountInfoVector.empty()){
        agentClient->exitConnection();
        return EXIT_FAILURE;
    }
    auto folderInfoVector = getDirsInfo(folders);
    std::vector<std::shared_ptr<modulog::space_agent::SpaceInfo>> allSpaceInfoVec(mountInfoVector);
    std::move(folderInfoVector.begin(), folderInfoVector.end(), std::back_inserter(allSpaceInfoVec));
    for (auto &spaceInfo: allSpaceInfoVec) {
        auto logMsg = std::make_shared<modulog::communication::LogMessage>(
                modulog::communication::LogMessage::LOG_MSG_TYPE::LOG, "totalSpaceMiB", spaceInfo->getTotalSpaceLog());
        agentClient->sendLog(logMsg);
    }

    while (agentClient->canLog()) {
        for (auto &spaceInfo: allSpaceInfoVec) {
            auto logMsg = std::make_shared<modulog::communication::LogMessage>(
                    modulog::communication::LogMessage::LOG_MSG_TYPE::LOG, "freeSpaceMiB",
                    spaceInfo->getAvailableSpaceLog());
            agentClient->sendLog(logMsg);
            if (percentTreshold > spaceInfo->getAvailablePercents()) {
                std::string toSend =
                        spaceInfo->getId() + " " + std::to_string(spaceInfo->getAvailablePercents()) + " %";
                auto errMsg = std::make_shared<modulog::communication::LogMessage>(
                        modulog::communication::LogMessage::LOG_MSG_TYPE::LOG, "errors", toSend);
                agentClient->sendLog(errMsg);

            }
        }
        agentClient->sleepFor(std::chrono::seconds(logInterval));
    }
    return 0;
}

