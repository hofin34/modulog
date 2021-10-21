#include <iostream>
#include<unistd.h>
#include <csignal>
#include <cstring>

void func_sigterm(int signum)
{
    printf("Caught!\n");
}

int main(){
    struct sigaction action;
    memset(&action, 0, sizeof(action));
    action.sa_handler = func_sigterm;
    sigaction(SIGTERM, &action, NULL);


    int i = 0;
    while(true){
        i++;
        unsigned int microsecond = 1000000;
        usleep(1 * microsecond);//sleeps for 3 second
        std::cout << "hello" << i << std::endl;
    }
    return 8;
}

