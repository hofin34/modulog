#include <modulog/watchdog_agent/WatchdogHandler.h>

namespace modulog::watchdog_agent{
    void WatchdogHandler::processMessage(const std::string &message) {

    }

    void WatchdogHandler::init(std::vector<std::string> devices) {
        for(auto &device : devices){
            //DeviceInfo deviceInfo{}
        }
    }
}
