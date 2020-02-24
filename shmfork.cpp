#include <iostream> 
#include <sys/ipc.h> 
#include <sys/shm.h> 
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <cstring>

using namespace std; 

int main() 
{ 
    // ftok to generate unique key 
    key_t key = ftok("shmfile",65); 
    
    // shmget returns an identifier in shmid 
    int shmid = shmget(key,1024,0666|IPC_CREAT); 
    
    // shmat to attach to shared memory 
    pid_t pid = fork();

    if(pid == 0)
    {
        char *str = (char*) shmat(shmid,(void*)0,0);
        while(*str != 'R')
        {
            usleep(1);
        }
        cout << "Output Data From PID " << getpid() << " : " << string(str +1) << endl; 
        shmdt(str); 
        shmctl(shmid,IPC_RMID,NULL); 
    }
    else if(pid > 0)
    {
        char *str = (char*) shmat(shmid,(void*)0,0); 
        cout << "Input Data for PID " << getpid() << " : ";
        string s;
        getline(cin, s);
        sprintf(str, "R%s", s.c_str());
        shmdt(str);
    }
    else
    {
        cout << "Error forking child process";
    }

    return 0; 
}
