#include <gtest/gtest.h>
//#include <modulog/communication/TcpServer.hpp>

// Demonstrate some basic assertions.
TEST(EasyTest, BasicAssertion) {
    // Expect equality.
    EXPECT_EQ(7 * 5, 35);
}

#include <modulog/meta_lib/SharedSettings.hpp>
#include <mutex>
#include <condition_variable>
#include <asio.hpp>
#include <modulog/core/Core.hpp>
#include <iostream>
#include <fstream>

TEST(CoreTest, BasicLog){
    std::ofstream enabledAgentsFile ("test-agents-enabled.conf");
    enabledAgentsFile << "" << std::endl;
    enabledAgentsFile.close();

    auto sharedSettings = std::make_shared<modulog::communication::SharedSettings>();
    sharedSettings->LogSettings.enabledAgentsPath = "test-agents-enabled.conf";
    try {
        auto ioContext = std::make_shared<asio::io_context>();
        auto core = std::make_unique<modulog::core::Core>(ioContext, sharedSettings);
        core->start();
    } catch (std::exception &e) {
        std::cerr << "Exception in test: " <<  e.what() << std::endl;
        exit(1);
    }
}
