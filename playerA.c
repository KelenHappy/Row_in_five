#include <stdio.h>
#include <stdlib.h>
#include <time.h>
typedef struct{
    int x;
    int y;
    int player;
}cord_t;
typedef struct{
    cord_t cord[100];
    int size;
}chess_t;
int setXY(chess_t *chess,int x,int y,int player){
    if (x == 0 || y == 0)return 0;
    char data[];
    for(int i=0;i<chess->size;i++){
        if(chess->cord[i].x == x && chess->cord[i].y == y)return 0;
    }
    chess->cord[cord->size].x = x;
    chess->cord[cord->size].y = y;
    chess->cord[cord-size].player = player;
    chess->size++;
    return 1;
void printChess(chess_t *chess){
    for(int i = 0,i < chess->size,i++){
        prinf("(%d %d %d)",chess->cord[i].x,chess->cord[i].y,chess->cord[i].player);
    }
    printf("size=%d\n",chess->size);
}

void writeBackServer(char *fileName,int x,int y ){
    File *f = fopen(fileName,"w");
    fprintf(f,"R\n");
    fprintf(f,"%d %d",x,y);
    fclose(f);
}
void writeChessBoard(chess_t *chess,int player,int *x,int *y){
    while(true){
        //write chess logic here
        if (setXY(chess,*x,*y,player) == 1)return;
    }
}
void go(char *fileName,chess_t chess){
    File *f = fopen(fileName,"r");
    char data =
}
int main(void){
    Chess chess;
    char *fileName = "a.txt";
    int count = 0;
    while(true){

        if (flag){
            count++;
            printf("%d",count);

            if (count > 50) break;

        }

    }
}
