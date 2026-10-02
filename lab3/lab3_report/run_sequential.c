#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main(int argc, char **argv) {
    if (argc < 4) {
        fprintf(stderr,
            "Usage: %s <array_size> <start> <end> [seed]\n", argv[0]);
        return 1;
    }

    pid_t pid = fork();
    if (pid < 0) { perror("fork"); return 1; }

    if (pid == 0) {
        char *args[] = {
            "./sequential_min_max",
            argv[1], argv[2], argv[3],
            (argc > 4) ? argv[4] : NULL,
            NULL
        };
        execv(args[0], args);
        perror("execv");
        _exit(127);
    }

    int status;
    if (waitpid(pid, &status, 0) < 0) {
        perror("waitpid");
        return 1;
    }

    if (WIFEXITED(status)) {
        printf("child exited with code %d\n", WEXITSTATUS(status));
    } else if (WIFSIGNALED(status)) {
        printf("child killed by signal %d\n", WTERMSIG(status));
    }

    return 0;
}
