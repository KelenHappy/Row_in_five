//player.c
#include "chessData.h"

void printChess(Chess* chess) {
    for (int i = 0; i < chess->size; i++) {
        printf("(%d, %d, %d) ", chess->cord[i].x, chess->cord[i].y, chess->cord[i].player);
    }
    printf("size=%d\n", chess->size);
}

void writeBackServer(const char* fileName, int x, int y) {
    FILE* f = fopen(fileName, "w");
    fprintf(f, "R\n");
    fprintf(f, "%d %d", x, y);
    fclose(f);
}

int go(const char* fileName, Chess* chess, char playerRole) {
    FILE* f = fopen(fileName, "r");
    char buffer[256];

    int player = 1;
    int co_player = 0;
    if (playerRole == 'A') {
        player = 0;
        co_player = 1;
    }

    if (fgets(buffer, sizeof(buffer), f) != NULL && buffer[0] == 'W') {
        int x, y;
        fscanf(f, "%d %d", &x, &y);

        if (player == 1 && x == 0 && y == 0) {
            fclose(f);
            return 0;
        }

        printf("%c:2: %d %d\n", playerRole, x, y);
        setXY(chess, x, y, co_player);

        int new_x, new_y;
        writeChessBoard(chess, player, &new_x, &new_y);
        printf("1: %d %d\n", new_x, new_y);
        writeBackServer(fileName, new_x, new_y);

        fclose(f);
        return 1;
    }

    fclose(f);
    return 0;
}

int main() {
    srand(time(NULL));
    Chess chess;
    initChess(&chess);

    char fileName[256];
    printf("fileName: ");
    scanf("%s", fileName);

    char playerRole;
    printf("Player A/B: ");
    scanf(" %c", &playerRole);

    int count = 0;
    while (1) {
        if (go(fileName, &chess, playerRole) == 1) {
            count++;
            printf("%d\n", count);
            printChess(&chess);
            if (count > 50) {
                break;
            }
        }
        sleep(3); // 3 seconds
    }

    return 0;
}
