#include <iostream>
#include <sys/time.h>
#include <cstring>
#include <cerrno>
#include <csignal>
#include <unistd.h>

static void handleTimer(int sig)
{
    switch(sig)
    {
        case SIGALRM:
        {
            std::cout << "Timer Expired" << std::endl;
        }
        break;

        default:
            std::cout << "Unexpected signal" << std::endl;
            break;
    }
}

int main(int argc, char **argv)
{
    int which = ITIMER_REAL;
    struct itimerval tint;

    if(signal(SIGALRM, handleTimer) == SIG_ERR)
    {
        std::cout << "Error - " << strerror(errno) << std::endl;
    }

    // Reload value for timer 
    // "it_value" upon expiration will be reloaded with "it_interval"
    tint.it_interval.tv_sec = 10;
    tint.it_interval.tv_usec = 0;

    // Initial timer interval 
    tint.it_value = tint.it_interval;

    // Start timer to generate "SIGALRM"
    // Every 10 seconds
    if(setitimer(which, &tint, NULL) < 0)
    {
        std::cout << "Error - " << strerror(errno) << std::endl;
    }

    while(1)
    {
        pause();
    }

    return 0;
}
