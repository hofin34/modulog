#include <iostream>
#include "subprocess.h"
#include <csignal>
#include <reproc++/drain.hpp>
#include <reproc++/reproc.hpp>
#include <utility>
#include <vector>
#include <cstring>
#include <filesystem>
#include <asio.hpp>


struct Agent{
    Agent(std::string name, int terminate_timeout, std::shared_ptr<reproc::process>& process):
    name(std::move(name)),
    process(process) {
        process_options.stop = {
                { reproc::stop::terminate, reproc::milliseconds(terminate_timeout) },
                { reproc::stop::kill, reproc::milliseconds(0) }
        };
    }
    // -------
    std::string name;
    std::filesystem::path path;
    std::shared_ptr<reproc::process> process;
    reproc::options process_options;
};
std::vector<Agent> running_agents;

void cleanup(int signum){
    std::cout << "cleanup..." << std::endl;
    for(auto &agent : running_agents){
        std::cout << "Stopping " << agent.name << " PID: " << agent.process->pid().first << std::endl;
        std::error_code ec;
        int status;
        std::tie(status, ec) = agent.process->stop(agent.process_options.stop);

    }
    exit(0);
}

using asio::ip::tcp;
class tcp_connection
        : public std::enable_shared_from_this<tcp_connection>
{
public:
    typedef std::shared_ptr<tcp_connection> pointer;

    static pointer create(asio::io_context& io_context)
    {
        return pointer(new tcp_connection(io_context));
    }

    tcp::socket& socket()
    {
        return socket_;
    }

    void start_alive_writer(){
        aliveTimer_.expires_from_now(asio::chrono::seconds(4));
        aliveTimer_.async_wait(std::bind(&tcp_connection::send_alive, this));
    }

    void send_alive(){
        asio::error_code errorWrite;
        std::cout << "sending message..." << std::endl;
        asio::write(socket_, asio::buffer(std::string("AreUAlive????\n")), errorWrite);
        std::cout << "err write: " << errorWrite << std::endl;
        waitingForACKAlive = true;
        aliveResponseTimer_.expires_from_now(asio::chrono::seconds(2));
        aliveResponseTimer_.async_wait(std::bind(&tcp_connection::alive_handler, this));
    }
    void alive_handler(){ // if is saved response -> check number -> if correct -> start_alive_writer() else send_alive
        if(waitingForACKAlive){
            //kill / reset
        }
    }

    void start_read()
    {
        // Start an asynchronous operation to read a newline-delimited message.
        asio::async_read_until(socket_, input_buffer_, '\n',
                                      std::bind(&tcp_connection::handle_read, shared_from_this(), std::placeholders::_1, std::placeholders::_2));
    }

private:
    tcp_connection(asio::io_context& io_context)
            : socket_(io_context), aliveTimer_(io_context), aliveResponseTimer_(io_context)
    {
    }

    void handle_read(const asio::error_code& error,
                      size_t bytes_transferred)
    {
        std::cout << "Read err_code: " << error << " Msg:  " << error.message() << std::endl;
        std::string line;
        std::istream is(&input_buffer_);
        std::getline(is, line);
        std::cout << "Rec data: " << line << std::endl; //TODO if isAliveAck -> isWaiting = false
        start_read();
    }
    bool waitingForACKAlive = false;
    asio::steady_timer aliveTimer_;
    asio::steady_timer aliveResponseTimer_;
    tcp::socket socket_;
    asio::streambuf input_buffer_;
};
// ------------------------
class tcp_server
{
public:
    tcp_server(asio::io_context& io_context)
            : io_context_(io_context),
              acceptor_(io_context, tcp::endpoint(tcp::v4(), 13))
    {
        start_accept();
    }

private:
    void start_accept()
    {
        tcp_connection::pointer new_connection =
                tcp_connection::create(io_context_);

        acceptor_.async_accept(new_connection->socket(),
                               std::bind(&tcp_server::handle_accept, this, new_connection,
                                           std::placeholders::_1));
    }

    void handle_accept(tcp_connection::pointer new_connection,
                       const asio::error_code& error)
    {
        if (!error)
        {
            std::cout << "Starting new connection..." << std::endl;
            new_connection->start_read();
            new_connection->start_alive_writer();
        }
        start_accept();
    }
    asio::io_context& io_context_;
    tcp::acceptor acceptor_;
};
int main(int argc, const char **argv) {
    try
    {
        asio::io_context io_context;
        tcp_server server(io_context);

        //std::thread thread1{[&io_context](){ io_context.run(); }};
        //std::thread thread2{[&io_context](){ io_context.run(); }};
        //thread1.join();
        //thread2.join();

        io_context.run();
    }
    catch (std::exception& e)
    {
        std::cerr << e.what() << std::endl;
    }

    return 0;
    /*
    struct sigaction act{};
    memset(&act, 0, sizeof(act));
    act.sa_handler = cleanup;
    sigaction(SIGINT,  &act, nullptr);
    sigaction(SIGTERM, &act, nullptr);

    // Agent definition:
    std::string ag1Name = "Agent 1";
    std::string ag1Path = "./process1";
    int ag1TerminateTimeout = 2000;
    // ---
    std::cout << "Core PID: " << getpid() << std::endl;
    reproc::options options;
    options.redirect.parent = true;
    auto process = std::make_shared<reproc::process>();
    std::vector<std::string> v;
    v.emplace_back(ag1Path);
    std::error_code ec = process->start(v, options);
    if (ec == std::errc::no_such_file_or_directory) {
        std::cerr << "Program not found. Make sure it's available from the PATH.";
        return ec.value();
    } else if (ec) {
        std::cout << "HERE..." << std::endl;
        return ec.value();
    }
    Agent agent(ag1Name, ag1TerminateTimeout, process);
    running_agents.push_back(std::move(agent));
    unsigned int microsecond = 1000000;
    usleep(1 * microsecond);//sleeps for 3 second
    while(true);
     */

}
