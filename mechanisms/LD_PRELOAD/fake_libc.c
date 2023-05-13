/**
 * Shared library, which is loaded via LD_PRELOAD.
 * Demonstrates how it is possible to override several
 * standard (and other) library calls to inject custom
 * methods during program execution
 */
#include <stdlib.h>

/* Functions to be injected */
int open(const char *pathname, int flags) {
    return -1;
}

void *opendir(const char *name) {
    return NULL;
}

int connect(int sockfd, void *addr, long long addrlen) {
    return -1;
}
