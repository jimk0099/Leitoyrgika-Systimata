/**
 * kill -SIGUSR1 
 * \file child.c
 * \author Aggelos Kolaitis <neoaggelos@gmail.com>
 * \date 2021-03-26
 * \brief execv() example, child process.
 */

#define RED "\033[31m"
#define GREEN "\033[32m"

#include <string.h>
#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <signal.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdbool.h>
#include <time.h>

char x1[10], x2[10];
int seconds = 0;
time_t t, live, start_timer;

/*
void message_open() {
  //printf(RED "[ID=%s/PID=%d/TIME=%ds] The gates are closed!\n", x, getpid(), seconds);
  for (seconds; seconds < 100; seconds++) {
    sleep(1);
    if(seconds % 15 == 0) {
      printf(GREEN "[ID=%s/PID=%d/TIME=%ds] The gates are open!\n", x, getpid(), seconds);
    }
  }
}

void message_closed() {
  //printf(RED "[ID=%s/PID=%d/TIME=%ds] The gates are closed!\n", x, getpid(), seconds);
  for (seconds; seconds < 100; seconds++) {
    sleep(1);
    if(seconds % 15 == 0) {
      printf(RED "[ID=%s/PID=%d/TIME=%ds] The gates are closed!\n", x, getpid(), seconds);
    }
  }
}
*/

void handle_child() {
  t = time(NULL);
  live = t - start_timer;
  if (strcmp(x2,"t") == 0) {
    printf(GREEN "[ID=%s/PID=%d/TIME=%lds] The gates are open!\n", x1, getpid(), live);
  }
  else {
    printf(RED "[ID=%s/PID=%d/TIME=%lds] The gates are closed!\n", x1, getpid(), live);
  }
  signal(SIGALRM, handle_child);
  alarm(15);
}

void handle_sigusr1(int sig) {
  t = time(NULL);
  live = t - start_timer;
  if (strcmp(x2,"t") == 0) {
    printf(GREEN "[ID=%s/PID=%d/TIME=%lds] The gates are open!\n", x1, getpid(), live);
    fflush(stdout);
  }
  else {
    printf(RED "[ID=%s/PID=%d/TIME=%lds] The gates are closed!\n", x1, getpid(), live);
    fflush(stdout);
  } 
}

void handle_sigusr2(int sig) {
  t = time(NULL);
  live = t - start_timer;
  if (strcmp(x2, "t") == 0) {
    strcpy(x2, "f");
    printf(RED "[ID=%s/PID=%d/TIME=%lds] The gates are closed!\n", x1, getpid(), live);
    fflush(stdout);
  }
  else {
    strcpy(x2, "t");
    printf(GREEN "[ID=%s/PID=%d/TIME=%lds] The gates are open!\n", x1, getpid(), live);
    fflush(stdout);
  }
}

int main(int argc, char **argv) {

  strcpy(x1, argv[1]);
  strcpy(x2, argv[2]);
  sleep(1);

  start_timer = time(NULL);
  live = 0;

  if (strcmp(x2,"t") == 0) {
    printf(GREEN "[ID=%s/PID=%d/TIME=%lds] The gates are open!\n", x1, getpid(), live);
  }
  else {
    printf(RED "[ID=%s/PID=%d/TIME=%lds] The gates are closed!\n", x1, getpid(), live);
  }

  struct sigaction sa;
  sa.sa_handler = &handle_sigusr1;
  sa.sa_flags = SA_RESTART;
  sigaction(SIGUSR1, &sa, NULL);

  struct sigaction sa2;
  sa2.sa_handler = &handle_sigusr2;
  sa2.sa_flags = SA_RESTART;
  sigaction(SIGUSR2, &sa2, NULL); 

  signal(SIGALRM, handle_child);
  alarm(15);

  while (1) {
    pause();
  }

  return 0;
} 
