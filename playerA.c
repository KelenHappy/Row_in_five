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
    for(int i=0;i<chess->size;++i){
        if(chess->cord[i].x == x && chess->cord[i].y == y)return 0;
    }
    chess->cord[cord->size].x = x;
    chess->cord[cord->size].y = y;
    chess->cord[cord-size].player = player;
    chess->size++;
    return 1;
void printChess(chess_t *chess){
    for(int i = 0,i < chess->size,++i){
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
    while(1){
        //write chess logic here
        if (setXY(chess,*x,*y,player) == 1)return;
    }
}
int go(char *fileName,chess_t chess){
    File *f = fopen(fileName,"r");
    char data[10];
    int player, co_player, x, y;
    player = 1;
    co_player = 0;
    if (fileName[0] == 'a') {
        player = 0;
        co_player = 1;
    }
    fgets(data, 10, f);
    if (data[0] == 'W') {
        fscanf(f, "%d %d", &x, &y);
        printf("2: %d %d\n", x, y);
        setXY(chess, x, y, co_player);
        writeChessBoard(chess, player, &x, &y);
        printf("1: %d %d\n", x, y);
        writeBackServer(fileName, x, y);
        fclose(f);
        return 1;
    }
    fclose(f);
    return 0;
}
int main(void){
    chess_t chess;
    chess.size = 0;
    char *fileName = "a.txt";
    srand(time(NULL));
    int count = 0,flag,x,y;
    while(1){
        flag = go(fileName,&chess)
        if (flag == 1){
            count++;
            printf("%d\n",count);
            printChess(&chess);
            if (count > 50) break;
        }
        sleep(2);
    }
    return 0;
}
