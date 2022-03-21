/*
 * Agent using linux-system-monitoring library from GitHub, that can monitor CPU, RAM, ... (look at README.md for more info)
 */

#include <modulog/agent_client/AgentClient.hpp>
#include <modulog/agent_client/Helpers.hpp>

#include <linux_monitoring/linux_memoryload.hpp>
#include <linux_monitoring/linux_cpuload.hpp>
#include <linux_monitoring/linux_process_load.hpp>
#include <linux_monitoring/util/timer.hpp>


int main(int argc, char **argv) {
    auto programStart = std::chrono::system_clock::now();
    nlohmann::json configJson = modulog::agent_client::Helpers::parseConfig(argv[0]);// nlohmann::json::parse(ifs);

    if (!configJson.contains("id")) {
        std::cerr << "Include config with id defined." << std::endl;
        return EXIT_FAILURE;
    }
    int cpuNotBiggerThanPercent = 0;
    if (configJson.contains("cpuNotBiggerThanPercent")) {
        cpuNotBiggerThanPercent = configJson["cpuNotBiggerThanPercent"];
    }
    int ramNotBiggerThanPercent = 0;
    if (configJson.contains("ramNotBiggerThanPercent")) {
        ramNotBiggerThanPercent = configJson["ramNotBiggerThanPercent"];
    }
    int logInterval = 4;
    if (configJson.contains("logInterval")) {
        logInterval = configJson["logInterval"];
    }


    auto ioContext = std::make_shared<asio::io_context>();
    modulog::agent_client::AgentClient agentClient(ioContext, configJson["id"]);
    agentClient.initClient();

    auto processes = std::make_unique<linux_monitoring::linuxProcessLoad>();
    auto cpuMonitoring = std::make_unique<linux_monitoring::cpuLoad>(configJson["statLocation"]);
    auto memoryMonitoring = std::make_unique<linux_monitoring::memoryLoad>();

    cpuMonitoring->initCpuUsage();
    auto maxRamToSend = std::make_shared<modulog::communication::LogMessage>(
            modulog::communication::LogMessage::LOG_MSG_TYPE::LOG, "deviceRamCapacityKb",
            std::to_string(memoryMonitoring->getTotalMemoryInKB()));
    agentClient.sendLog(maxRamToSend);

    // print cpu usage of all cpu cores
    linux_monitoring::util::Timer::periodicShot([&] {
        auto currRam = memoryMonitoring->getCurrentMemUsageInPercent();
        std::shared_ptr<modulog::communication::LogMessage> ramToSend;
        if (currRam <= ramNotBiggerThanPercent)
            ramToSend = std::make_shared<modulog::communication::LogMessage>(
                    modulog::communication::LogMessage::LOG_MSG_TYPE::LOG, "usedRamPercent", std::to_string(currRam));
        else
            ramToSend = std::make_shared<modulog::communication::LogMessage>(
                    modulog::communication::LogMessage::LOG_MSG_TYPE::ERROR, "usedRamPercent", std::to_string(currRam));
        agentClient.sendLog(ramToSend);
        auto usedRamToSendKb = std::make_shared<modulog::communication::LogMessage>(
                modulog::communication::LogMessage::LOG_MSG_TYPE::LOG, "usedRamKb",
                std::to_string(memoryMonitoring->getCurrentMemUsageInKB()));
        agentClient.sendLog(usedRamToSendKb);


        auto currCpu = cpuMonitoring->getCurrentCpuUsage();
        std::shared_ptr<modulog::communication::LogMessage> toSend;
        if (currCpu <= cpuNotBiggerThanPercent)
            toSend = std::make_shared<modulog::communication::LogMessage>(
                    modulog::communication::LogMessage::LOG_MSG_TYPE::LOG, "usedAvgCpu", std::to_string(currCpu));
        else
            toSend = std::make_shared<modulog::communication::LogMessage>(
                    modulog::communication::LogMessage::LOG_MSG_TYPE::ERROR, "usedAvgCpu", std::to_string(currCpu));
        agentClient.sendLog(toSend);

        auto cpus = cpuMonitoring->getCurrentMultiCoreUsage();
        for (int i = 0; i < cpus.size(); i++) {
            auto cpuId = std::string("usedCpu" + std::to_string(i));
            auto cpuX = std::make_shared<modulog::communication::LogMessage>(
                    modulog::communication::LogMessage::LOG_MSG_TYPE::LOG, cpuId, std::to_string(cpus.at(i)));
            agentClient.sendLog(cpuX);
        }
    }, std::chrono::seconds(logInterval));

    while (linux_monitoring::util::Timer::isRunning()) {
        std::this_thread::sleep_for(std::chrono::seconds(std::numeric_limits<int>::max()));
    }
    linux_monitoring::util::Timer::stop();
    return 0;
}

