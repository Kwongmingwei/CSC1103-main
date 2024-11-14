#include <stdio.h> 
#include <stdlib.h>
#include <stdbool.h> 

#define P1 'O'
#define P2 'X'

void startGame();
void showBoard(char board[3][3]);
char checkWinner(char board[3][3]);
int choice;

int main(){
    while (true){
        while (true){
        choice=0; 
        printf("\n[1] Player against bot\n");
        printf("[2] Player against Player\n");
        printf("[3] Exit\n");
        scanf("%d",&choice);
        printf("Player chose option %d",choice);
        if (choice==3){
            break;
        }
        else if(choice==1){
             
        }

        else if(choice==2){
            startGame();
        }
        else{
            printf("Invalid choice\n");
        }
    }
	
    }
    return 0; 
}

void startGame(){
    char board[3][3]; 
    int movesLeft=9;
    int turn=1;
    for (int i = 0; i < 3; i++) { 
        for (int j = 0; j < 3; j++) {
            board[i][j] = ' '; 
        }
    } 
    
    while (true){
        if (checkWinner(board)!=' '||movesLeft==0){
            break;
        }
        showBoard(board);
        int move,row,column;
        printf("Player %d:Enter your move (1-9): ",turn); 
        scanf("%d", &move); 
        if (move < 1 || move > 9) { 
                printf("Please enter a value between 1-9.\n"); 
                continue; 
        }
        row=(move-1)/3;
        column=(move-1)%3;
        if (board[row][column] == ' ') { 
            if (turn==1){
                board[row][column] = P1; 
                if (checkWinner(board)!=' '){
                    break;
                }
                turn=2;
            }
            else{
                board[row][column] = P2; 
                if (checkWinner(board)!=' '){
                    break;
                }
                turn=1;
            }
            movesLeft--;
        }
        else{
            printf("Cell %d is already occupied. Please choose another cell\n", move);
        }
                
    }
    showBoard(board);
    if (movesLeft==0){
        printf("It's a draw\n"); 
    }
    else{
        int playerwon=0;
        if (checkWinner(board)==P1){
            playerwon=1;
        }
        else if(checkWinner(board)==P2){
            playerwon=2;
        }
        else{
            printf("None won");
        }
        printf("Game over, Player %d has won\n",playerwon); 
    }


    
}

void showBoard(char board[3][3]) 
{ 
    printf("\n\n"); 
    for (int i = 0; i < 3; i++) { 
        printf("%c | %c | %c", board[i][0],board[i][1], board[i][2]); 
        printf("\n-----------\n");        
    } 
} 

char checkWinner(char board[3][3]){
    for (int i = 0; i < 3; i++) { 
        if (board[i][0] == board[i][1]&& board[i][1] == board[i][2] && board[i][0] != ' '){
            return board[i][0]; 
        }
        if (board[0][i] == board[1][i]&& board[1][i] == board[2][i] && board[0][i] != ' '){
            return board[0][i];
        }
    } 
    if ((board[0][0] == board[1][1] && board[1][1] == board[2][2] && board[0][0] != ' ') || (board[0][2] == board[1][1] && board[1][1] == board[2][0] && board[0][2] != ' ')){
            return board[1][1];
    }
    return ' '; 
}