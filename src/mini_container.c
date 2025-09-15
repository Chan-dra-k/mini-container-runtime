#define _GNU_SOURCE
#include <sched.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/mount.h>

#define STACK_SIZE (1024*1024)
static char *child_stack;

int child_fn(void *arg) {
    sethostname("miniOS", 6);
    // try mounting proc (requires root)
    mount("proc", "/proc", "proc", 0, "");
    printf("Inside container: hostname -> ");
    char h[100];
    gethostname(h, sizeof(h));
    printf("%s\n", h);
    // show PID of self
    printf("Inside container: pid=%d\n", getpid());
    execlp("ps", "ps", "-ef", NULL);
    return 0;
}

int main() {
    child_stack = malloc(STACK_SIZE);
    if (!child_stack) { perror("malloc"); exit(1); }
    int flags = CLONE_NEWPID | CLONE_NEWUTS | CLONE_NEWNS | SIGCHLD;
    pid_t pid = clone(child_fn, child_stack + STACK_SIZE, flags, NULL);
    if (pid == -1) { perror("clone"); exit(1); }
    printf("[host] created child pid: %d\n", pid);
    waitpid(pid, NULL, 0);
    printf("[host] child finished\n");
    return 0;
}


