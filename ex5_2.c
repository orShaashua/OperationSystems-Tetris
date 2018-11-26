#include <stdio.h>
#include <stdbool.h>
#include <termio.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>

#define SIZE_OF_BOARD 20
#define SIZE_OF_PLAYER

void updatePlayer(int sig);
void alarm_down(int sig);
void changePosition();
void goLeft();
void goRight();
void goDown();
void initializePlayer();
void initializeBoard();
void cleanBoard();
void updateBoard();
void printBoard();





typedef enum position{vertical, horizontal}Position;

typedef struct player{
    int startX;
    int endX;
    int startY;
    int endY;
    Position position;


}Player;


// Global board and game piece
char gameBoard[SIZE_OF_BOARD][SIZE_OF_BOARD];
Player globalPlayer;


int main() {
    signal(SIGUSR2, updatePlayer);
    signal(SIGALRM, alarm_down);
    initializePlayer();
    initializeBoard();
    printBoard();

    //
    alarm(1);
    while (true){
        pause();
    }



}


void printBoard(){
    int i,j;
    system("clear");
    for (i = 0;i < SIZE_OF_BOARD; i++ ){
        for(j = 0;j < SIZE_OF_BOARD; j++){
            printf("%c",gameBoard[i][j]);
        }
        printf("\n");
    }
}


void initializePlayer(){
    globalPlayer.position = horizontal;
    globalPlayer.startY = globalPlayer.endY = 1;
    globalPlayer.startX = SIZE_OF_BOARD/2 -1;
    globalPlayer.endX = globalPlayer.startX + 2;
}

void initializeBoard(){
    int i,j;
    for (i = 0;i < SIZE_OF_BOARD; i++ ){
        for(j = 0;j < SIZE_OF_BOARD; j++){
            if(i == 0 || i == SIZE_OF_BOARD-1
               || j==SIZE_OF_BOARD-1 || j == 0){
                gameBoard[i][j]= '*';
            }else{
                gameBoard[i][j] = ' ';
            }
        }
    }
    gameBoard[globalPlayer.startY][globalPlayer.startX] = '-';
    gameBoard[globalPlayer.startY][globalPlayer.startX + 1] = '-';
    gameBoard[globalPlayer.endY][globalPlayer.endX] = '-';
}


void updatePlayer(int sig){
    signal(SIGUSR2 ,updatePlayer);
    char ch = (char) getchar();
    switch (ch) {
        case 'a':
            goLeft();
            break;
        case 'd':
            goRight();
            break;
        case 's':
            goDown();
            break;
        case 'w':
            changePosition();
            break;
        case 'q':
            exit(0);
        default:
            break;
    }
    printBoard();
}

void changePosition(){
    cleanBoard();
    if (globalPlayer.position == horizontal){
        if((globalPlayer.endY != SIZE_OF_BOARD -2) && (globalPlayer.endY != 1)) {
            globalPlayer.position = vertical;
            globalPlayer.startX = globalPlayer.startX + 1;
            globalPlayer.endX = globalPlayer.endX - 1;
            globalPlayer.startY = globalPlayer.startY - 1;
            globalPlayer.endY = globalPlayer.endY + 1;
        }
    }else{
        if((globalPlayer.endX != SIZE_OF_BOARD - 2) && (globalPlayer.startX != 1)) {
            globalPlayer.position = horizontal;
            globalPlayer.startX = globalPlayer.startX - 1;
            globalPlayer.endX = globalPlayer.endX + 1;
            globalPlayer.startY = globalPlayer.startY + 1;
            globalPlayer.endY = globalPlayer.endY - 1;
        }
    }
    updateBoard();
}

void cleanBoard(){
    int i;
    if (globalPlayer.position == horizontal ){
        for (i = globalPlayer.startX;i<= globalPlayer.endX;i++){
            gameBoard[globalPlayer.startY][i] = ' ';
        }
    }else {
        for (i = globalPlayer.startY;i<= globalPlayer.endY;i++){
            gameBoard[i][globalPlayer.startX] = ' ';
        }
    }
}

void updateBoard(){
    int i;
    if (globalPlayer.position == horizontal ){
        for (i = globalPlayer.startX;i<= globalPlayer.endX;i++){
            gameBoard[globalPlayer.startY][i] = '-';
        }
    }else {
        for (i = globalPlayer.startY;i<= globalPlayer.endY;i++){
            gameBoard[i][globalPlayer.startX] = '-';
        }
    }
}


void goLeft(){
    cleanBoard();
    if(globalPlayer.startX != 1){
        globalPlayer.startX = globalPlayer.startX - 1;
        globalPlayer.endX = globalPlayer.endX -1;
    }
    updateBoard();
}


void goRight(){
    cleanBoard();
    if(globalPlayer.endX != SIZE_OF_BOARD - 2){
        globalPlayer.startX = globalPlayer.startX + 1;
        globalPlayer.endX = globalPlayer.endX + 1;
    }
    updateBoard();
}

void goDown(){
    cleanBoard();
    if(globalPlayer.endY != SIZE_OF_BOARD - 2){
        globalPlayer.startY = globalPlayer.startY + 1;
        globalPlayer.endY = globalPlayer.endY + 1;
    }else{
        initializePlayer();
    }
    updateBoard();
}

void alarm_down(int sig) {
    signal(SIGALRM, alarm_down);
    goDown();
    printBoard();
    alarm(1);
}