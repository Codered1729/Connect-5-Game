#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

#define SIZE 8
#define EMPTY ' '
#define MAX_SCORES 100

typedef struct {
    char name[50];
    int wins;
} PlayerScore;



void initBoard(char board[SIZE][SIZE]) {
    for (int i = 0; i < SIZE; i++)
        for (int j = 0; j < SIZE; j++)
            board[i][j] = EMPTY;
}

void printBoard(char board[SIZE][SIZE]) {
    printf("\n  ");
    for (int c = 0; c < SIZE; c++)
        printf(" %d", c + 1);
    printf("\n");
    for (int r = 0; r < SIZE; r++) {
        printf("%d ", SIZE - r);
        for (int c = 0; c < SIZE; c++)
            printf("|%c", board[r][c]);
        printf("|\n");
    }
    printf("  ");
    for (int c = 0; c < SIZE; c++) printf("--");
    printf("-\n\n");
}

int getLowestEmptyRow(char board[SIZE][SIZE], int col) {
    for (int r = SIZE - 1; r >= 0; r--)
        if (board[r][col] == EMPTY)
            return r;
    return -1;
}

int isWinningMove(char board[SIZE][SIZE], int row, int col, char sym) {
    int dirs[4][2] = {{0,1},{1,0},{1,1},{1,-1}};
    for (int d = 0; d < 4; d++) {
        int cnt = 1;
        // forward
        for (int step = 1; step < 5; step++) {
            int rr = row + dirs[d][0]*step,
                cc = col + dirs[d][1]*step;
            if (rr<0||rr>=SIZE||cc<0||cc>=SIZE||board[rr][cc]!=sym) break;
            cnt++;
        }
        // backward
        for (int step = 1; step < 5; step++) {
            int rr = row - dirs[d][0]*step,
                cc = col - dirs[d][1]*step;
            if (rr<0||rr>=SIZE||cc<0||cc>=SIZE||board[rr][cc]!=sym) break;
            cnt++;
        }
        if (cnt >= 5) return 1;
    }
    return 0;
}

int makeMove(char board[SIZE][SIZE], char sym, int col) {
    int row = getLowestEmptyRow(board, col);
    if (row < 0) return 0;
    board[row][col] = sym;
    return 1;
}

// Simple AI: win if possible, block if needed, else pick tallest column.
int getBestMove(char board[SIZE][SIZE], char comp, char player) {
    // try win
    for (int c = 0; c < SIZE; c++) {
        int r = getLowestEmptyRow(board, c);
        if (r<0) continue;
        board[r][c] = comp;
        if (isWinningMove(board, r, c, comp)) {
            board[r][c] = EMPTY;
            return c;
        }
        board[r][c] = EMPTY;
    }
    // block
    for (int c = 0; c < SIZE; c++) {
        int r = getLowestEmptyRow(board, c);
        if (r<0) continue;
        board[r][c] = player;
        if (isWinningMove(board, r, c, player)) {
            board[r][c] = EMPTY;
            return c;
        }
        board[r][c] = EMPTY;
    }
    // best available
    int bestC = -1, bestR = -1;
    for (int c = 0; c < SIZE; c++) {
        int r = rand()%8;
        if (r > bestR) {
            bestR = r;
            bestC = c;
        }
    }
    return bestC;
}

// ---------- Scoreboard Functions ----------

void loadScores(PlayerScore arr[], int *cnt) {
    FILE *f = fopen("scores.txt","r");
    *cnt=0;
    if(!f) return;
    while(*cnt<MAX_SCORES &&
          fscanf(f,"%49s %d",arr[*cnt].name,&arr[*cnt].wins)==2) {
        (*cnt)++;
    }
    fclose(f);
}

void saveScores(PlayerScore arr[], int cnt) {
    FILE *f = fopen("scores.txt","w");
    if(!f) { perror("scores.txt"); return; }
    for(int i=0;i<cnt;i++)
        fprintf(f,"%s %d\n", arr[i].name, arr[i].wins);
    fclose(f);
}

int compareScores(const void *a, const void *b) {
    return ((PlayerScore*)b)->wins - ((PlayerScore*)a)->wins;
}

void updateScore(const char *winner) {
    PlayerScore arr[MAX_SCORES];
    int cnt=0, found=0;
    loadScores(arr,&cnt);
    for(int i=0;i<cnt;i++){
        if(strcmp(arr[i].name,winner)==0){
            arr[i].wins++;
            found=1;
            break;
        }
    }
    if(!found && cnt<MAX_SCORES){
        strncpy(arr[cnt].name,winner,49);
        arr[cnt].name[49]='\0';
        arr[cnt].wins=1;
        cnt++;
    }
    saveScores(arr,cnt);
}

void printScores() {
    PlayerScore arr[MAX_SCORES];
    int cnt=0;
    loadScores(arr,&cnt);
    if(cnt==0){
        printf("No scores yet.\n");
        return;
    }
    qsort(arr,cnt,sizeof(PlayerScore),compareScores);
    printf("\n--- Scoreboard ---\n");
    for(int i=0;i<cnt;i++){
        printf("%s: %d wins\n", arr[i].name, arr[i].wins);
    }
    printf("------------------\n\n");
}

// ---------- Single Round Runner ----------

void playRound(int mode, const char *p1, const char *p2) {
    char board[SIZE][SIZE];
    initBoard(board);
    printBoard(board);

    int turn = 0;  // 0->p1, 1->p2/comp

    while(1) {
        int col;
        if(turn==0) {
            printf("%s (X), choose column 1-%d: ", p1, SIZE);
            scanf("%d",&col); col--;
            if(col<0||col>=SIZE||!makeMove(board,'X',col)) {
                printf("Invalid. Try again.\n");
                continue;
            }
        }
        else {
            if(mode==1) {
                // computer
                col = getBestMove(board,'O','X');
                if(col<0) { printf("No moves left.\n"); break; }
                makeMove(board,'O',col);
                printf("Computer (O) chose %d\n",col+1);
            } else {
                printf("%s (O), choose column 1-%d: ", p2, SIZE);
                scanf("%d",&col); col--;
                if(col<0||col>=SIZE||!makeMove(board,'O',col)) {
                    printf("Invalid. Try again.\n");
                    continue;
                }
            }
        }

        printBoard(board);

        // check win
        int won=0;
        for(int r=0;r<SIZE;r++){
            for(int c=0;c<SIZE;c++){
                char sym = board[r][c];
                if(sym!=EMPTY && isWinningMove(board,r,c,sym)){
                    won=1;
                    if(turn==0){
                        printf("%s wins!\n", p1);
                        updateScore(p1);
                    } else {
                        if(mode==1){
                            printf("Computer wins!\n");
                            updateScore("Computer");
                        } else {
                            printf("%s wins!\n", p2);
                            updateScore(p2);
                        }
                    }
                    break;
                }
            }
            if(won) break;
        }
        if(won) break;

        // check draw
        int draw=1;
        for(int c=0;c<SIZE;c++){
            if(getLowestEmptyRow(board,c)!=-1){
                draw=0;
                break;
            }
        }
        if(draw){
            printf("It's a draw!\n");
            break;
        }

        turn = 1 - turn;
    }

    printScores();
}

// ---------- Main Menu Loop ----------

int main(){
    srand((unsigned)time(NULL));

    int choice;
    char p1[50], p2[50];

    do {
        printf("\n=== CONNECT-5 MAIN MENU ===\n");
        printf("1) Single Player\n");
        printf("2) Two Players\n");
        printf("3) View Scoreboard\n");
        printf("4) Exit\n");
        printf("Select an option: ");
        scanf("%d",&choice);

        switch(choice){
        case 1:
            printf("Enter your name: ");
            scanf("%49s",p1);
            strcpy(p2,"Computer");
            playRound(1,p1,p2);
            break;

        case 2:
            printf("Enter Player 1 name: ");
            scanf("%49s",p1);
            printf("Enter Player 2 name: ");
            scanf("%49s",p2);
            playRound(2,p1,p2);
            break;

        case 3:
            printScores();
            break;

        case 4:
            printf("Goodbye!\n");
            break;

        default:
            printf("Invalid choice. Please pick 1–4.\n");
        }

    } while(choice!=4);

    return 0;
}
