#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char **argv) {
    int forkCheck;
    int pfd[2];

    pipe(pfd);

    forkCheck = fork();

    if (forkCheck != 0) {
        dup2(pfd[1], 1);
        close(pfd[0]);
        if (argc > 1) {
            execlp("ls", "ls", "-1a", argv[1], NULL);
            perror("directory");
            exit(1);
        } else {
            execlp("ls", "ls", "-1a", NULL);
            perror("ls");
            exit(1);
        }
    } else {
        dup2(pfd[0], 0);
        close(pfd[1]);
        execlp("wc", "wc", "-l", NULL);
        perror("wc");
        exit(1);
    }
}