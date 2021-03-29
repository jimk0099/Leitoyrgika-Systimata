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

#define DEFAULT "\033[30;1m"
#define RED "\033[31;1m"
#define GREEN "\033[32m"
#define YELLOW "\033[33m"
#define BLUE "\033[34m"
#define MAGENTA "\033[35m"
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

void forker(int nprocesses) {
    pid_t pid;
    
    if(nprocesses > 0) {
        if ((pid = fork()) < 0) {
            perror("fork");
        }
        else if (pid == 0) {
            //Child stuff here
            if(buffer[w] == 't') {
                printf("Child %d with pid = %d with gates OPEN\n", w, pidtable[w] = getpid());
            }
            else {
                printf("Child %d with pid = %d with gates CLOSED\n", w, pidtable[w] = getpid());
                printf("My buffer is %d\n", buffer[w]);
            }
        }
        else if(pid > 0) {
            //parent
            w++;
            sleep(1);
            forker(nprocesses - 1);
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
    printf("I will create %d children\n", j-1);
    forker(j-1);
    printf("w = %d, pid = %d\n", w, pidtable[w]);
    return 0;
}