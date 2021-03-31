#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <time.h>
#include <sys/times.h>
#include <stdbool.h>

#define DEFAULT "\033[30;1m"
#define RED "\033[31;1m"
#define GREEN "\033[32;1m"
#define YELLOW "\033[33m"
#define BLUE "\033[34m"
#define MAGENTA "\033[35;1m"
#define CYAN "\033[36m"
#define WHITE "\033[37m"
#define GRAY "\033[38;1m"

/* File Descriptors for stdin and stdout */
#define FD_STDIN 0
#define FD_STDOUT 1
#define FD_STDERR 2

/* Arbitrary buffer size */
#define BUFFER_SIZE 64

/* User read-write, group read, others read */
#define PERMS 0644

int j = 0, w = 0;
pid_t pidtable[100];
char buffer[100];
bool gate_state[100];

void forker(int nprocesses) {
    pid_t pid;
    
    if(nprocesses > 0) {
        if ((pid = fork()) < 0) {
            perror("fork");
            exit(-1);
        }
        else if (pid == 0) {
            //Child stuff here
            pidtable[w] = getpid();
            if(buffer[w] == 't') {
                gate_state[w] = true;
                sleep(2);
                printf(GREEN "[ID=%d/PID=%d/TIME=...] The gates are open!\n", w, pidtable[w]);
            }
            else {
                gate_state[w] = false;
                sleep(2);
                printf(RED "[ID=%d/PID=%d/TIME=...] The gates are closed!\n", w, pidtable[w]);
                //printf("My buffer is %d\n", buffer[w]);
            }
        }
        else if(pid > 0) {
            //parent
            sleep(1);
            kill(pid, SIGSTOP);
            printf(MAGENTA "[PARENT/PID=%d] Created child %d (PID=%d) and initial state %d\n", getpid(), w, pidtable[w], gate_state[w]);
            w++;
            forker(nprocesses - 1);
            for(int z = 0; z < w; z++) {
                kill(pidtable[z], SIGCONT);
                sleep(1);
            }
        }
    }
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        perror("too many arguments");
        exit(-1);
    }
    buffer[strlen(argv[1])];
    j = snprintf(buffer, strlen(argv[1])+1, "%s\n", argv[1]);
    //printf("%s\ncount = %d\n", buffer, j-1);                                                                        //prints argv string & number of children                                                      
    for (int i = 0; buffer[i] != '\0'; i++) {
        if ((buffer[i] == 't' || buffer[i] != 'f') && (buffer[i] != 't' || buffer[i] == 'f')) {
            perror("wrong arguments");
            exit(-1);
        }
    }
    //printf("I will create %d children\n", j-1);
    forker(j-1);
    //printf("w = %d, pid = %d\n", w, pidtable[w]);
    return 0;
}