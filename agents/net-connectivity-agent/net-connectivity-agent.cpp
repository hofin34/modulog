/*
 * modulog agent, that monitors net connectivity (look at README.md for more info)
 */

#include <modulog/communication/MessageSerializer.hpp>
#include <modulog/agent_client/AgentClient.hpp>
#include <modulog/agent_client/Helpers.hpp>

#include <thread>
#include <iostream>
#include <sys/types.h>

bool hostnameToIp(std::string &ip, const std::string &hostname) {

    struct hostent *he;
    struct in_addr **addrList;
    if ((he = gethostbyname(hostname.c_str())) == nullptr) {
        return false;
    }
    addrList = (struct in_addr **) he->h_addr_list;

    if (addrList != nullptr && addrList[0] != nullptr) {
        ip = std::string(inet_ntoa(*addrList[0]));
        return true;
    } else {
        return false;
    }
}

bool isInternet() {
    int sock;
    struct sockaddr_in serv_addr{};
    if ((sock = socket(AF_INET, SOCK_STREAM | SOCK_NONBLOCK, 0)) < 0) {
        return false;
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(80); // connect_ to port 80
    std::string ip;
    if (!hostnameToIp(ip, "google.com")) {
        return false;
    }

    // Convert address from text to binary form
    if (inet_pton(AF_INET, ip.c_str(), &serv_addr.sin_addr) <= 0) {
        return false;
    }


    // tries to connect_
    int connectRes = connect(sock, (struct sockaddr *) &serv_addr, sizeof(serv_addr));
    if (connectRes == -1 && errno == EINPROGRESS) {
        std::cout << "Connected: OK" << std::endl;
    }
    // ---Trying to read data---
    // setting select():
    std::string msg = "GET /unknown/file HTTP/1.0\r\n\r\n";
    std::this_thread::sleep_for(std::chrono::seconds(2));
    const int sendRes = send(sock, msg.c_str(), strlen(msg.c_str()), 0);
    if (sendRes == -1) {
        std::cerr << "Cannot send data." << std::endl;
        return false;
    }

    fd_set set;
    struct timeval timeout{};
    int ret_val_select;

    timeout.tv_sec = 2;
    timeout.tv_usec = 0;
    FD_ZERO(&set);
    FD_SET(sock, &set);

    ret_val_select = select(sock + 1, &set, nullptr, nullptr, &timeout);
    if (ret_val_select == -1) {
        std::cerr << "select() error" << std::endl;
        return false;
    } else if (ret_val_select == 0) {
        std::cerr << "Read data: TIMEOUT" << std::endl;
        return false;
    } else {
        const int BUFFER_SIZE = 128;
        char buffer[BUFFER_SIZE] = {0};
        ssize_t valread = read(sock, buffer,
                               BUFFER_SIZE); // We want to read just some bytes to see if connection is ok
        if (valread > 0) {
            std::cout << "Read data: OK" << std::endl;
            return true;
        } else {
            std::cerr << "Read data: ERR" << std::endl;
            return false;
        }
    }
}


void logNetState(bool stateValue, modulog::agent_client::AgentClient &agentClient) {
    std::shared_ptr<modulog::communication::LogMessage> msg;
    if (stateValue == true) {
        msg = std::make_shared<modulog::communication::LogMessage>(
                modulog::communication::LogMessage::LOG_MSG_TYPE::LOG, "isInternetConnectivity",
                std::to_string(stateValue));
    } else {
        msg = std::make_shared<modulog::communication::LogMessage>(
                modulog::communication::LogMessage::LOG_MSG_TYPE::ERROR, "isInternetConnectivity",
                std::to_string(stateValue));
    }
    agentClient.sendLog(msg);
}

int main(int argc, char **argv) {
    auto programStart = std::chrono::system_clock::now();
    nlohmann::json configJson = modulog::agent_client::Helpers::parseConfig(argv[0]);// nlohmann::json::parse(ifs);
    if (!configJson.contains("id"))
        throw std::runtime_error("Include config with id defined.");
    int checkInterval = 4;
    if (configJson.contains("checkInterval")) {
        checkInterval = configJson["checkInterval"];
    }

    auto ioContext = std::make_shared<asio::io_context>();
    modulog::agent_client::AgentClient agentClient(ioContext, false, configJson["id"]);
    agentClient.initClient();
    auto lastNetState = isInternet();
    logNetState(lastNetState, agentClient);

    while (true) {
        std::this_thread::sleep_for(std::chrono::seconds(checkInterval));
        auto newNetState = isInternet();
        if (newNetState != lastNetState) {
            logNetState(newNetState, agentClient);
            lastNetState = newNetState;
        }
    }
    return 0;
}

