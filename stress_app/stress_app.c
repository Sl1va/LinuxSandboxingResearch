/**
 * Simple application which calculates sum of elements
 * of 2 MATRIX_SIZE x MATRIX_SIZE sized matrices and 
 * performs some system calls after each iteration.
 * 
 * Creates userspace and kernel space load and can be
 * used to measure how different sandboxing tools are
 * affecting application performance. 
 */
#include <stdio.h>
#include <stdint.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>


#define MATRIX_SIZE 70

int main() {
    int64_t m1[MATRIX_SIZE][MATRIX_SIZE];
    int64_t m2[MATRIX_SIZE][MATRIX_SIZE];
    int64_t sum = 0;

    for (int i = 0; i < MATRIX_SIZE; ++i) {
        for (int j = 0; j < MATRIX_SIZE; ++j) {
            for (int k = 0; k < MATRIX_SIZE; ++k) {
                for (int l = 0; l < MATRIX_SIZE; ++l) {
                    sum += m1[i][j] * m2[k][l];
                    
                    /* Try to perform some system calls */
                    getpid();
                    getgid();
                    getuid();
                }
            }
        }    
    }
}