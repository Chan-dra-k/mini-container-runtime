#define _GNU_SOURCE
#include <sched.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

#define STACK_SIZE (1024*1024)
static char *child_stack;

int child_fn(void *arg) {
    printf("Hello from container! pid=%d\n", getpid());
    sleep(2);
    return 0;
}

int main() {
    child_stack = malloc(STACK_SIZE);
    if (!child_stack) { perror("malloc"); exit(1); }
    int flags = CLONE_NEWPID | CLONE_NEWUTS | SIGCHLD;
    pid_t pid = clone(child_fn, child_stack + STACK_SIZE, flags, NULL);
    if (pid == -1) { perror("clone"); exit(1); }
    printf("[host] created child pid: %d\n", pid);
    waitpid(pid, NULL, 0);
    printf("[host] child finished\n");
    return 0;
}
