#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define MEMORY_BLOCK_SIZE (1024 * 1024) // 1 MB

int main() {
    pid_t pid = getpid();
    printf("PID: %d\n", pid);

    char input[10];
    printf("Enter a 1 to start: ");
    fgets(input, sizeof(input), stdin);

    if (input[0] != '1') return 0;

    // Infinite loop to consume CPU and Allocate memory indefinitely to consume memory
    while (1) {
        void* memory_block = malloc(MEMORY_BLOCK_SIZE);
        memset(memory_block, 0, MEMORY_BLOCK_SIZE);
    }

    return 0;
}