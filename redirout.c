#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

int main(int argc, char **argv) {
    int forkCheck;
    int pfd[2];
    char *index = sizeof char;
    char *buffer[2048] = {};
    int bytes;
    int fd;
    
    if (argc <= 1) {
        perror("invalid usage");
        exit(1);
    }

    fd = open(argv[1], O_RDWR|O_CREAT|O_TRUNC, 0644);

    pipe(pfd);

    forkCheck = fork();

    if (forkCheck == 0) {
        dup2(pfd[1], 1);
        close(pfd[0]);
        execvp(argv[2], argv[2] + index);
        perror("child exec");
    } else {
        close(pfd[1]);
        while((bytes = read(pfd[0], buffer, 2048)) > 0) {
            write(fd, buffer, bytes);
        }
        close(pfd[0]);
        close(fd);
    }
}