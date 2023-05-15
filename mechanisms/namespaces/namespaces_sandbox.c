#define _GNU_SOURCE
#define STACK_SIZE (1024 * 1024)

#include <sched.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/mount.h>
#include <sys/wait.h>

char child_stack[STACK_SIZE];
  int child_pid;

int child_fn(void *arg) {
  char *const argv[] = { "/bin/bash", NULL };
  char *const envp[] = { NULL };
  execve(argv[0], argv, envp);
  perror("execve");
  return 1;
}

void set_up_namespaces(void) {
    // Unshare the mount namespace and remount / as private
    if (unshare(CLONE_NEWNS) == -1) {
        perror("unshare(CLONE_NEWNS)");
        return;
    }
    if (mount("none", "/", NULL, MS_REC | MS_PRIVATE, NULL) == -1) {
        perror("mount");
        return;
    }

    // Unshare the network namespace
    if (unshare(CLONE_NEWNET) == -1) {
        perror("unshare(CLONE_NEWNET)");
        return;
    }
    printf("Namespaces activated\n");

    // Configure the network interface for the new namespace
  system("ip link set lo up");
  system("ip link add veth0 type veth peer name veth1");
  system("ip link set veth1 netns 1");
  system("ip addr add 192.168.0.1/24 dev veth0");
  system("ip link set veth0 up");
  system("ip route add default via 192.168.0.1 dev veth0");

  // Disable network connection for the new namespace
  system("ip link set veth0 down");
  system("ip link del veth0");

    // Fork the child process, specifying the child function to run
  child_pid = clone(child_fn, child_stack + STACK_SIZE, SIGCHLD, NULL);
  if (child_pid == -1) {
    perror("clone");
    return 1;
  }

  // Wait for the child process to exit
  waitpid(child_pid, NULL, 0);
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("Usage: %s EXECUTABLE\n", argv[0]);
        return 1;
    }

    char *executable_path = argv[1];
    
    set_up_namespaces();
}