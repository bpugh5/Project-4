#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

int main(int argc, char **argv) {
    int forkCheck;
    int pfd[2];
    char buffer[2048];
    int bytes;
    int fd;

    if (argc <= 2) {
        printf("Not enough arguments, please try again! :)\n"
        "Usage: redirout  outfilename  command opt1 opt2 ...\n");
        return 1;
    }

    fd = open(argv[1], O_RDWR|O_CREAT|O_TRUNC, 0644);

    pipe(pfd);

    forkCheck = fork();

    if (forkCheck == 0) {
        dup2(pfd[1], 1);
        close(pfd[0]);
        execvp(argv[2], argv + 2);
        perror("child exec");
        exit(1);
    } else {
        close(pfd[1]);
        while((bytes = read(pfd[0], buffer, 2048)) > 0) {
            write(fd, buffer, bytes);
        }
        close(pfd[0]);
        close(fd);
    }
}