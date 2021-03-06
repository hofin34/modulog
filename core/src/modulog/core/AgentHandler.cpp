#include <modulog/core/AgentHandler.hpp>

namespace modulog::core{
    AgentHandler::AgentHandler(const std::filesystem::path& pathToEnabledAgentsList): runningAgents_(){
        parseEnabledAgentsList(pathToEnabledAgentsList);
    };


    std::shared_ptr<AgentProcess> AgentHandler::runNextAgent() {
        if(createdAgentsCount_ >= agentsPaths_.size()){
            return nullptr;
        }
        std::filesystem::path actAgentFolder = std::filesystem::absolute(agentsPaths_.at(createdAgentsCount_));
        std::string folderName = actAgentFolder.filename();
        std::filesystem::path execPath = actAgentFolder/folderName;
        if(!std::filesystem::exists(execPath)){
            std::cerr << "Executable " << execPath << " not found" << std::endl;
            throw std::runtime_error("Executable " + std::string(execPath) + " not found");
        }
        // Agent definition:
        std::string agentId = actAgentFolder; // is replaced after communication with client with real id from client side
        std::string agentPath = execPath;
        // ---

        auto options = std::make_shared<reproc::options>();
        options->stop = {
                { reproc::stop::kill, reproc::milliseconds(0) }
        };
        options->redirect.parent = true;
        auto process = std::make_shared<reproc::process>();
        std::vector<std::string> v;
        v.emplace_back(agentPath);
        std::error_code ec = process->start(v, *options);
        createdAgentsCount_++;
        if (ec == std::errc::no_such_file_or_directory) {
            throw std::runtime_error("Program not found. Make sure it's available from the PATH");
        } else if (ec) {
            throw std::runtime_error("Agent creation err:" + ec.message());
        }
        auto agentInfo = std::make_shared<AgentProcess>(agentId, agentPath, process, options);
        return agentInfo;
    }


    void AgentHandler::addNewAgent(const std::shared_ptr<communication::MessageExchanger> &messageExchanger, const std::shared_ptr<AgentProcess> &agentInfo){
        auto agent = std::make_shared<Agent>(agentInfo, messageExchanger);
        runningAgents_.push_back(agent);
    }

    const std::vector<std::shared_ptr<Agent>> &AgentHandler::getRunningAgents() {
        return runningAgents_;
    }

    void AgentHandler::parseEnabledAgentsList(const std::filesystem::path& pathToListFile) {
        std::ifstream input( pathToListFile);
        if(!input.is_open())
            throw std::runtime_error(pathToListFile.generic_string() + " cannot be opened. (maybe not existing...)");
        for( std::string line; getline( input, line ); )
        {
            line.erase(remove(line.begin(), line.end(), ' '), line.end());
            int commentPos = line.find('#');
            if(commentPos != std::string::npos) { // line contains comment
                line = line.substr(0, commentPos);
            }
            if(line.empty())
                continue;
            std::filesystem::path actAgentPath = line;
            agentsPaths_.push_back(actAgentPath);
        }
    }

    void AgentHandler::deleteAgent(const std::shared_ptr<Agent>& agent) {
        { // deletes agent from vector
            auto it = std::find(runningAgents_.begin(), runningAgents_.end(), agent);
            if (it != runningAgents_.end()) {
                agent->deleteSelf();
                runningAgents_.erase(it);
            }
        }
    }




}
