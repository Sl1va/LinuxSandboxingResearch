#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <seccomp.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/wait.h>

void set_up_seccomp(void) {
    /* Create seccomp context with allow-all mode */
    scmp_filter_ctx ctx = seccomp_init(SCMP_ACT_ALLOW);

    /* Add `openat` and `connect` syscalls to blacklist */   
    seccomp_rule_add(ctx, SCMP_ACT_ERRNO(EPERM), SCMP_SYS(openat), 0);
    seccomp_rule_add(ctx, SCMP_ACT_ERRNO(EPERM), SCMP_SYS(connect), 0);

    /* Apply seccomp rules */
    seccomp_load(ctx);
    printf("Seccomp activated\n");
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("Usage: %s EXECUTABLE\n", argv[0]);
        return 1;
    }

    char *executable_path = argv[1];
    
    int pid = fork();
    if (pid == -1) {
        perror("Failed to fork\n");
        exit(EXIT_FAILURE);
    } else if (pid == 0) {
        /* Prepare seccomp */
        set_up_seccomp();

        /* Execvp binary */
        char *args[] = {executable_path, NULL};
        execvp(executable_path, args);

        /* execvp failed */
        perror("Failed to perform execvp");
        exit(EXIT_FAILURE);
    } else {
        /* Wait until child is done */
        wait(NULL);
    }
}