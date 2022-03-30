#include <modulog/watchdog_agent/WatchdogHandler.hpp>

namespace modulog::watchdog_agent {
    void WatchdogHandler::processMessage(const std::string &message) {
        const std::regex msgRegex("^[a-zA-Z0-9]+\\_[0-9]+$");
        std::cout << "rec msg: " << message << std::endl;
        if (!std::regex_match(message, msgRegex)) {
            std::cerr << "Bad msg format" << std::endl;
            auto errMsg = std::make_shared<communication::LogMessage>(communication::LogMessage::LOG_MSG_TYPE::ERROR,
                                                                      "errorMessage", "bad device msg format:" + message);
            agentClient_->sendLog(errMsg);
            return;
        }
        std::lock_guard<std::mutex> lock(mtx_);
        std::string delimiter = "_";
        int delimPos = message.find(delimiter);
        std::string deviceName = message.substr(0, delimPos);
        std::string timestampStr = message.substr(delimPos + delimiter.length());
        uint64_t timestamp = std::stoi(timestampStr);
        for (auto &dev: deviceInfoVector_) {
            if (dev.getName() == deviceName) {
                dev.setSentMessage(true);
                dev.setTimestamp(timestamp);
            }
        }

    }

    void WatchdogHandler::init(std::vector<std::string> devices) {
        for (auto &deviceName: devices) {
            DeviceInfo deviceInfo{deviceName, agentClient_};
            deviceInfoVector_.push_back(deviceInfo);
        }
        checkoutTimer_.expires_from_now(std::chrono::seconds(checkoutInterval_));
        checkoutTimer_.async_wait([this](const asio::error_code &ec) { checkAllDevices(ec); });
    }

    void WatchdogHandler::checkAllDevices(const asio::error_code &) {
        std::lock_guard<std::mutex> lock(mtx_);
        for (auto &device: deviceInfoVector_) {
            if (device.getSentMessage() == false) {
                device.nextInactive();
            }
            device.setSentMessage(false);
        }
        checkoutTimer_.expires_from_now(std::chrono::seconds(checkoutInterval_));
        checkoutTimer_.async_wait([this](const asio::error_code &ec) { checkAllDevices(ec); });
    }
}
