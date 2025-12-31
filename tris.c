#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

//player struct
struct Player {
char name[5];
char mark;
bool type;
int winCount;
};
//introduction message with mode selection
bool printIntro (void) {
    int mode = 0;
    //welcomes player and prompts choice
    printf("Welcome to Tris by Gigi!\nDo you want to play alone or with a friend? Insert '1' to play alone or '2' to play with a friend:\n");
    do{
        if(scanf("%d", &mode) != 1){ //check for number
            printf("Error: Invalid input!\nInsert 1 to play against the CPU or 2 to play against your friend:\n");
        }
        else if(mode != 1 && mode != 2) { //check if number is in the correct range
            printf("Error: Invalid choice!\nInsert 1 to play against the CPU or 2 to play against your friend:\n");
        }
        while(getchar() != '\n');
    } while(mode != 1 && mode != 2);
    return (mode == 1); //return true if 1 and false if 2
}
//prints the board
void printBoard(char board[3][3]) {
    printf("\033[H\033[2J"); //ansi escape sequence to clear the screen
    printf("Tris by Gigi\n************\n");
    for(int i = 0; i < 3; i++) {
        printf("=========\n");
        for(int j = 0; j < 3; j++) {
            printf("|%c|", board[i][j]);
        }
        printf("\n");
    }
    printf("=========\n");
}
//enters the name of the player(s)
void getName(struct Player* player) {
    printf("Insert player name: (max 4 characters)\n");
    //inserts name of the player; accepts anything
    if(fgets(player->name, 5, stdin)) {
        size_t len = strlen (player->name); //takes dimension of player->name
        if(len > 0 && player->name[len - 1] == '\n') { //check for \n (enter key) in last element
            player->name[len - 1] = '\0'; //input the end string
            int c;
            while ((c = getchar()) != '\n' && c != EOF); //cleans the buffer
        } 
        else {
            int c;
            while((c = getchar()) != '\n' && c != EOF); //cleans the buffer
        }
        if(player->name[0] == '\0') {
            strcpy(player->name, "Anon\0");
        }
    }
}
//checks if win condition is met
int checkWin(char board[3][3]) {
    for(int i = 0; i < 3; i++) {
        if((board[i][0]) == board[i][1] && board [i][1] == board[i][2]) {
            return 1;
        }
        else if((board[0][i] == board[1][i]) && (board[1][i] == board[2][i])) {
            return 1;
        }
    }
    if((board[0][0] == board [1][1]) && (board[1][1] == board [2][2])) {
        return 1;
    }
    else if((board[0][2] == board [1][1]) && (board[1][1] == board [2][0])) {
        return 1;
    }
    return 0;
}
//manages the logic for the turns
int manageTurns(char board[3][3], struct Player player) {
    int move = 0;
    if(player.type == true) {
        printBoard(board);
        printf("Select the spot (1-9):\n");
        if(scanf("%d", &move) != 1) { //check if input is number
            while(getchar() != '\n'); 
            return 1;
        }
        else {
            while (getchar() != '\n'); 
            if(move >= 1 && move <= 9) { //check if range is correct
                int x = (move - 1) / 3; //convertin move to x/y
                int y = (move + 2) % 3;
                if(board[x][y] != 'X' && board[x][y] != 'O') { //check if spot is empty
                    board[x][y] = player.mark;
                    return 0;
                }
                else {
                    return 3;
                }
            }
            else {
                return 2;
            }
       }
    }
    else {
        move = (rand() % 9) + 1; //randomise the move
        int x = (move - 1) / 3; //convertin move to x/y
        int y = (move + 2) % 3;
        if(board[x][y] != 'X' && board[x][y] != 'O') { //check if spot is empty
            board[x][y] = player.mark;
            return 0;
        }
        else {
            return 2;
        }
    }
}
//asks for rematch
bool askRematch() {
    char choice;
    printf("Do you want to play again? (insert 'y' to continue or any other key to close)\n");
    scanf("%c", &choice);
    while(getchar() != '\n');
    if(choice == 'y') {
        return true;
    }
    else {
        return false;
    }
}

int main () {
    //initialise srand and main variables
    srand(time(NULL));
    struct Player player1, player2;
    bool win = false;
    //select the mode
    if(printIntro() == true) {
        printf("You chose to play against the CPU!\n");
        //initialise the players
        player1.mark = 'X';
        player2.mark = 'O';
        getName(&player1);
        strcpy(player2.name, "CPU");
        player1.type = true;
        player2.type = false;
        player1.winCount = 0;
        player2.winCount = 0;
    }
    else {
        printf("You chose to play against a friend!\n");
        //initialise the players
        player1.mark = 'X';
        player2.mark = 'O';
        getName(&player1);
        getName(&player2);
        player1.type = true;
        player2.type = true;
        player1.winCount = 0;
        player2.winCount = 0;
    }
    //game start
    do {
        char board[3][3] = {{'1', '2', '3'}, {'4', '5', '6'}, {'7', '8', '9'}};
        int moveCount = 0;
        do {
            int outcome;
            if(moveCount % 2 == 0) {
                outcome = manageTurns(board, player1);
            }    
            else if(moveCount % 2 == 1) {
                outcome = manageTurns(board, player2);
            }
            //check for outcome of turn
            switch(outcome) {
                case 0:
                    win = checkWin(board);
                    moveCount++;
                    break;
                case 1: 
                    printf("Error: Not a number!\n");
                    break;
                case 2: 
                    printf("Error: Out of range!\n");
                    break;
                case 3:
                    printf("Error: Spot taken!\n");
                    break;
                default: 
                    printf("Unexpected error :(\n");
                    break;
            }
        }while(win == 0 && moveCount < 9);
        //game over
        printBoard(board);
        if(win == 1) {
            if ((moveCount-1) % 2 == 0) {
                printf("%s won!\n", player1.name);
                player1.winCount++;
            }
            else {
                printf("%s won!\n", player2.name);
                player2.winCount++;
            }
            printf("Score: %s %d - %d %s\n", player1.name, player1.winCount, player2.winCount, player2.name);
        }
        else {
            printf("Draw!\n");
        }
    }while(askRematch() == true);
    //thanks the player(s) :)
    printf("Thanks for playing!\n");

    return 0;
}