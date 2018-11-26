#include <stdio.h>
#include <unistd.h>
#include <termios.h>
#include <bits/signum.h>
#include <signal.h>
#include <string.h>

#define EXE_GAME "./draw.out"
#define STDERR 2
#define ERROR_MSG "Error in system call\n"

char getch() {
    char buf = 0;
    struct termios old = {0};
    if (tcgetattr(0, &old) < 0)
        perror("tcsetattr()");
    old.c_lflag &= ~ICANON;
    old.c_lflag &= ~ECHO;
    old.c_cc[VMIN] = 1;
    old.c_cc[VTIME] = 0;
    if (tcsetattr(0, TCSANOW, &old) < 0)
        perror("tcsetattr ICANON");
    if (read(0, &buf, 1) < 0)
        perror ("read()");
    old.c_lflag |= ICANON;
    old.c_lflag |= ECHO;
    if (tcsetattr(0, TCSADRAIN, &old) < 0)
        perror ("tcsetattr ~ICANON");
    return (buf);
}
void printError() {
    write(STDERR, ERROR_MSG, sizeof(ERROR_MSG));
}

int main() {
    int Pipe[2];
    pipe(Pipe);
    int pid;

    if ((pid = fork()) < 0) {
        printError();
    }

    // process of child
    if (pid == 0) {
        // force our stdin to be the read size of the pipe we made
        dup2(Pipe[0], 0);
        //play the tetris program
        execlp(EXE_GAME, EXE_GAME, NULL);
        // Gets here only if exelp failed.
        printError();
    }

    //process father
    char ch;
    while (1) {
        //check which char enter the user
        ch = getch();
        // write the key to the pipe
        if(write(Pipe[1], &ch, 1) < 0) {
            printError();
        }
        //kill the process
        kill(pid, SIGUSR2);
        //check if ch is QUIT
        if (ch == 'q') {
            break;
        }
    }


    return 0;
}