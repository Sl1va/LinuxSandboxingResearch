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
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <dirent.h>

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

/**
 * @brief Try to read /etc/passwd,
 *        print first several lines from the file
 * 
 * @return true On success
 * @return false On fail
 */
static bool read_file(void) {
    int fd = open("/etc/passwd", O_RDONLY);

    if (fd == -1) {
        printf("Failed to open /etc/passwd\n");
        return false;
    }
    printf("Opened /etc/passwd\n");

    char buf[512];
    int r = read(fd, buf, sizeof(buf));
    close(fd);

    if (r == -1) {
        printf("Failed to read from file descriptor\n");
        return false;
    }
    // print only first line or full buffer if
    // line consist of more than 512 symbols
    buf[sizeof(buf) - 1] = '\0';
    for (int i = 0; i < sizeof(buf) - 1; ++i) {
        if (buf[i] == '\n') {
            buf[i] = '\0';
            break;
        }
    }

    printf("First line of /etc/passwd:\n%s\n", buf);
    return true;
}
REGISTER_ACTIVITY(read_file);

/**
 * @brief List /proc directory and count how many processes
 *        are running at the moment
 * 
 * @return true On success
 * @return false On fail
 */
static bool count_processes(void) {
    DIR *dir = opendir("/proc");
    if (!dir) {
        printf("Failed to open /proc directory\n");
        return false;
    }

    int64_t cnt = 0;
    struct dirent *entry;
    while ((entry = readdir(dir)) != NULL) {
        int64_t pid;
        if (sscanf(entry->d_name, "%ld", &pid)) ++cnt;
    }
    printf("%ld processes detected\n", cnt);
    
    closedir(dir);
    return cnt;
}
REGISTER_ACTIVITY(count_processes);

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
        printf("=== %s start ===\n\n", activities[i].name);
        if (activities[i].func()) {
            ++success;
            printf("\n=== %s success ===\n\n", activities[i].name);
        } else
            printf("\n=== %s fail ===\n\n", activities[i].name);
    }
    printf("\nSummary: %d succeed, %d failed\n", success, (int) num_activities - success);
}