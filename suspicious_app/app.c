/**
 * Simple C app which performs some suspicious activity
 * 
 * Note: All suspicious activities are located in section
 * "Suspicious activities", no need to touch other parts of code 
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/**
 * @brief Function should return true if suspicous activity 
 *        was successfully performed, and false otherwise
 */
typedef bool (*suspicious_act_func)(void);

typedef struct {
    const char *name;
    suspicious_act_func func;
} suspicious_act_t;

suspicious_act_t *activities = NULL;
size_t num_activities = 0;

static void push_activity(const char *name, suspicious_act_func activity) {
    activities = realloc(activities, sizeof(suspicious_act_t) * (++num_activities));
    activities[num_activities - 1] = (suspicious_act_t) {
        .name = name,
        .func = activity,
    };
}

#define INIT __attribute__((constructor))
#define REGISTER_ACTIVITY(act)          \
    INIT void register_##act(void) {    \
        push_activity(#act, act);       \
    }

/**************************************************
 * Suspicious activities start
 **************************************************/ 

static bool read_file(void) {
    return false;
}
REGISTER_ACTIVITY(read_file);

static bool list_processes(void) {
    return false;
}
REGISTER_ACTIVITY(list_processes);

static bool access_network(void) {
    return false;
}
REGISTER_ACTIVITY(access_network);

/**************************************************
 * Suspicious activities end
 **************************************************/ 

int main() {
    int success = 0;
    for (int i = 0; i < num_activities; ++i) {
        printf("=== %s start ===\n", activities[i].name);
        if (activities[i].func()) {
            ++success;
            printf("=== %s success ===\n\n", activities[i].name);
        } else
            printf("=== %s fail ===\n\n", activities[i].name);
    }
    printf("\nSummary: %d succeed, %d failed\n", success, (int) num_activities - success);
}