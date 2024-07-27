#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"


int main(int argc, char const *argv[])
{
    int pid;
    int pipe1[2], pipe2[2];
    pipe(pipe1);
    pipe(pipe2);
    char buf[1];

    pid = fork();
    if (pid == 0) {
        close(pipe1[1]); // close write end of pipe1
        close(pipe2[0]); // close read end of pipe2
        if (read(pipe1[0], buf, 1) != 1) {
            fprintf(2, "child read error\n");
            exit(1);
        }
        printf("%d: received ping\n", getpid());
        close(pipe1[0]); // close read end of pipe1
        write(pipe2[1], "1", 1);
        close(pipe2[1]); // close write end of pipe2
    } else {
        close(pipe1[0]); // close read end of pipe1
        close(pipe2[1]);    // close write end of pipe2
        write(pipe1[1], "1", 1);
        close(pipe1[1]); // close write end of pipe1
        if (read(pipe2[0], buf, 1) != 1) {
            fprintf(2, "parent read error\n");
            exit(1);
        }
        printf("%d: received pong\n", getpid());
        close(pipe2[0]); // close read end of pipe2
    }

    return 0;
}
