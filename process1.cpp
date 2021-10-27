#include <iostream>
#include<unistd.h>
#include <csignal>
#include <cstring>

void signal_handler(int signum)
{
    std::cout << "Caught signal "<< signum << std::endl;
}

int main(){

    struct sigaction action{};
    memset(&action, 0, sizeof(action));
    action.sa_handler = signal_handler;
    sigaction(SIGTERM, &action, nullptr);
    sigaction(SIGINT, &action, nullptr);


    int i = 0;
    while(true){
        i++;
        unsigned int microsecond = 1000000;
        usleep(1 * microsecond);//sleeps for 1 second
        std::cout << "hello" << i << std::endl;
    }
}

