#include "logic.h"
#include <stdio.h>

#ifndef AI_H
#define AI_H
#define max_rows 958
#define max_cols 9

Cell AITurn();

static char table_for_board[max_rows][max_cols];
static int table_for_outcome[max_rows];
static char board_train[max_rows][max_cols];
static char board_test[max_rows][max_cols];
static int outcome_train[max_rows];
static int outcome_test[max_rows];
static float class_count[2];
static float likelihood[max_rows][max_cols][3];
static char currentboard[max_cols];

int Minimax(Cell grid[COLS][ROWS], bool isMaximizing, int depth, int maxDepth);
void load_data(char X[max_rows][max_cols], int y[max_rows]);
void split_data(char X[max_rows][max_cols], int y[max_rows], char X_train[max_rows][max_cols], int y_train[max_rows], char X_test[max_rows][max_cols], int y_test[max_rows], int data_size);
void trainNaiveBayes(char X_train[max_rows][max_cols], int y_train[max_rows], int train_size,float prior[2], float likelihood[max_rows][max_cols][3]);
float predictNaiveBayes(char X[], float prior[2], float likelihood[2][9][3]);
void GetBoard(Cell grid[COLS][ROWS], char board[9]);

void load_data(char X[max_rows][max_cols], int y[max_rows]){

    FILE *file_ptr; //File pointer
    char coordinate1,coordinate2,coordinate3,coordinate4,coordinate5,coordinate6,coordinate7,coordinate8,coordinate9; //Variables to store content of each tic tac toe box
    static char outcome[8] = {}; //Store the outcome of the board configuration

    if ((file_ptr = fopen("tic-tac-toe.data","rb")) == NULL) //File pointer does not open any file
    {

        printf("input File could not be opened\n"); //If file cannot be found
        
    }

    else

    {

        for (int i = 0; i < max_rows; i++) // Loop to store each row of data in table_for_board and table_for_outcome
        {

            // find ways to make it not hardcoded
            fscanf(file_ptr,"%c,%c,%c,%c,%c,%c,%c,%c,%c,%s\n",&coordinate1,&coordinate2,&coordinate3,&coordinate4,
            &coordinate5,&coordinate6,&coordinate7,&coordinate8,&coordinate9,&*outcome);

            X[i][0] = coordinate1;
            X[i][1] = coordinate2;
            X[i][2] = coordinate3;
            X[i][3] = coordinate4;
            X[i][4] = coordinate5;
            X[i][5] = coordinate6;
            X[i][6] = coordinate7;
            X[i][7] = coordinate8;
            X[i][8] = coordinate9;

            if (outcome[0] == 'p')

            {

                y[i] = 1; 

            }

            else if (outcome[0] = 'n')
            {
                y[i] = 0;
            }

            //{insert close file pointer}

        }

    }

}

// function to split data into 80% training and 20% testing
void split_data(char X[max_rows][max_cols], int y[max_rows], 
                char X_train[max_rows][max_cols], int y_train[max_rows], 
                char X_test[max_rows][max_cols], int y_test[max_rows], 
                int data_size) {

    //int train_size = (int)(data_size * (1 - 0.2));

    // Variables to store the number of cases
    int win_count = 0;
    int lose_count = 0;

    // for loop to count total number of positive and negative cases
    for (int i = 0; i < max_rows; i++) {

        if (y[i] == 1)
        {
            win_count += 1;
        }
        

    }

    // for loop to count total number of negative cases
    for (int i = 0; i < max_rows; i++) {

        if (y[i] == 0)
        {
            lose_count += 1;
        }
        

    }

    // see whether can use round to round up or down to nearest int
    int train_win = (int) (win_count * 0.8);
    int train_lose = (int) (lose_count * 0.8);
    int train_size = train_win + train_lose;
    
    // for loop to store the positive cases into training set
    for (int i = 0; i < train_win; i++) {

        for (int j = 0; j < max_cols; j++) {

            X_train[i][j] = X[i][j];

        }

        y_train[i] = y[i];

    }

    // for loop to store the negative cases into training set
    for (int i = train_win; i < train_size; ++i) {

        for (int j = 0; j < max_cols; ++j) {

            X_train[i][j] = X[i][j];

        }

        y_train[i] = y[i];
    }

    // count the total number of cases used for testing
    int win_test_size = win_count - train_win;
    int lose_test_size = lose_count - train_lose;
    int test_size = win_test_size + lose_test_size;

    // for loop to store the positive cases into testing set
    for (int i = 0; i < win_test_size; ++i) {

        for (int j = 0; j < max_cols; ++j) {

            X_test[i][j] = X[i][j];

        }

        y_test[i] = y[i];
    }

    // for loop to store the negative cases into testing set
    for (int i = win_test_size; i < test_size; ++i) {

        for (int j = 0; j < max_cols; ++j) {

            X_test[win_test_size+i][j] = X[win_test_size+i][j];

        }

        y_test[win_test_size+i] = y[win_test_size+i];
    }

}

//Training of Bayes Model
void trainNaiveBayes(char X_train[max_rows][max_cols], int y_train[max_rows], int train_size,
                     float prior[2], float likelihood[max_rows][max_cols][3]) {
                        
    // Calculate prior probabilities for each class (win or lose)max
    
    float classcount[2] = {0,0};
    
    for (int i = 0; i < train_size; i++) {

        if (y_train[i] == 0)
        {

            classcount[0] += 1;
        
        }

        else{

            classcount[1] += 1;
        }
        
        
    }

    prior[0] = (float) (classcount[0] / train_size);  // P(Lose)
    prior[1] = (float) (classcount[1] / train_size);  // P(Win)

    // Calculate likelihoods P(Xi | Y) for each feature and class

    // for each class
    for (int i = 0; i < 2; i++) {

        // for each column
        for (int j = 0; j < max_cols; j++) {

            int count[3] = {0,0,0};

            // for each row
            for (int k = 0; k < train_size; k++) {

                if (y_train[k] == i) {

                    if (X_train[k][j] == 'x')
                    {
                        count[0] += 1;
                    }

                    else if (X_train[k][j] == 'o')
                    {
                        count[1] += 1;
                    }

                    else
                    {
                        count[2] += 1;
                    }

                }
            }

            for (int k = 0; k < 3; k++) {

                //come back later
                likelihood[i][j][k] = (float) (count[k]) / classcount[i];  // P(Xi = k | Y = i)

            }
        }
    }
}

float predictNaiveBayes(char X[], float prior[2], float likelihood[2][9][3]) {
    float posteriors[2] = {0};

    // Compute the posterior probability for each class
    
    for (int i = 0; i < 2; i++) {
        posteriors[i] = prior[i];  // Start with the prior P(Y)
        for (int j = 0; j < 9; j++) {
            char feature_value = X[j];
            if (feature_value == 'x')
            {

                posteriors[i] = posteriors[i] * likelihood[i][j][0]; // Multiply by the likelihood P(Xj | Y)
                
            }
            else if (feature_value == 'o')
            {

                posteriors[i] = posteriors[i] * likelihood[i][j][1]; // Multiply by the likelihood P(Xj | Y)
                
            }
            else
            {

                posteriors[i] = posteriors[i] * likelihood[i][j][2]; // Multiply by the likelihood P(Xj | Y)
        
            }

        }

    }

    return posteriors[0];


}

void GetBoard(Cell grid[COLS][ROWS], char board[9])
{
    for (int i = 0; i < COLS; i++)
    {
        for (int j = 0; j < ROWS; j++)
        {
            if (grid[i][j].value == EMPTY)
            {
                board[j*3+i] = 'b';
            }
            
            else
            {
                if (grid[i][j].value == X)
                {
                    board[j*3+i] = 'x';
                }
                else if (grid[i][j].value == O)
                {
                    board[j*3+i] = 'o';
                }
                
            }
            
        }
        
    }
    
}

// Minimax Function
int Minimax(Cell grid[COLS][ROWS], bool isMaximizing, int depth, int maxDepth)
{
    CellValue winner;
    if (GameIsOver(&winner))
    {
        if (winner == O)
            return 1; // AI wins
        if (winner == X)
            return -1; // Human wins
        return 0;      // Tie
    }

    if (tilesOccupied == 9 || depth >= maxDepth) 
    {
        return 0; // Return 0 as a simple evaluation when max depth is reached or grid is full (Tie if all cells are filled)
    }

    int bestScore = isMaximizing ? -1000 : 1000;

    for (int i = 0; i < COLS; i++)
    {
        for (int j = 0; j < ROWS; j++)
        {
            if (grid[i][j].value == EMPTY)
            {
                // Simulate the move
                grid[i][j].value = isMaximizing ? O : X;
                tilesOccupied++;

                int score = Minimax(grid, !isMaximizing, depth + 1, maxDepth);

                // Undo the move
                grid[i][j].value = EMPTY;
                tilesOccupied--;

                // Choose the best score
                if (isMaximizing)
                {
                    bestScore = (score > bestScore) ? score : bestScore;
                }
                else
                {
                    bestScore = (score < bestScore) ? score : bestScore;
                }
            }
        }
    }
    return bestScore;
}

Cell AITurn()
{
    Cell retCell;
    if (difficulty == 1) // EASY 
    {
        
        float currentprobability = 0;
        float bestprobability = 0;
        int bestprobabilityi = -1;
        int bestprobabilityj = -1;

        for (int i = 0; i < COLS; i++)
        {
            for (int j = 0; j < ROWS; j++)
            {
                if (grid[i][j].value == EMPTY) // Check if cell is empty
                {
                    // Simulate the move by temporarily selecting it
                    grid[i][j].value = O;
                    GetBoard(grid,currentboard);
                    currentprobability = predictNaiveBayes(currentboard,class_count,likelihood);

                    // Undo the move and reset the board to simulate other moves
                    grid[i][j].value = EMPTY;

                    // Check if the current move is better
                    if (currentprobability > bestprobability)
                    {
                        bestprobability = currentprobability; // update best score
                        bestprobabilityi = i; // Track row of best move
                        bestprobabilityj = j; // Track column of best move
                    }
                }
            }

            if (bestprobabilityi != -1 && bestprobabilityj != -1) // Mark the best move found
            {
                // Execute the best move found
                retCell.i=bestprobabilityi;
                retCell.j=bestprobabilityj;
                retCell.value = O;
            }
            
        }

    }
    else if (difficulty == 2) // MED
    {

        int bestScore = -1000;
        int bestMoveI = -1;
        int bestMoveJ = -1;
        int maxDepth = 2; // Minimax will only predict 2 moves in advance

        // Loop through all cells to find the best possible mov
        for (int i = 0; i < COLS; i++)
        {
            for (int j = 0; j < ROWS; j++)
            {
                if (grid[i][j].value == EMPTY)  // Check if cell is empty
                {
                    // Simulate the move by temporarily selecting it
                    grid[i][j].value = O;
                    tilesOccupied++; // Tracking of occupied cells

                    // Call the minimax function with maximum depth of 2 (imperfect minimax with restricted depth) to make it possible for the player to win
                    int moveScore = Minimax(grid, false, 0, maxDepth);

                    // Undo the move and reset the board to simulate other moves
                    grid[i][j].value = EMPTY;
                    tilesOccupied--;

                    // Check if the current move is better
                    if (moveScore > bestScore)
                    {
                        bestScore = moveScore; // update best score
                        bestMoveI = i; // Track row of best move
                        bestMoveJ = j; // Track column of best move
                    }
                }
            }
        }

        if (bestMoveI != -1 && bestMoveJ != -1) // Mark the best move found
        {
            // Execute the best move found
            retCell.i=bestMoveI;
            retCell.j=bestMoveJ;
            retCell.value=O;
        }
        
    }
    else if (difficulty == 3) // Hard
    {
        int bestScore = -1000;
        int bestMoveI = -1;
        int bestMoveJ = -1;

        // Loop through all cells to find the best possible mov
        for (int i = 0; i < COLS; i++)
        {
            for (int j = 0; j < ROWS; j++)
            {
                if (grid[i][j].value == EMPTY)  // Check if cell is empty
                {
                    // Simulate the move by temporarily selecting it
                    grid[i][j].value = O;
                    tilesOccupied++; // Tracking of occupied cells

                    // Call the minimax function with full depth (Perfect AI with no restrictions) impossible for the player to win, only tie
                    int moveScore = Minimax(grid, false, 0, INT_MAX);

                    // Undo the move and reset the board to simulate other moves
                    grid[i][j].value = EMPTY;
                    tilesOccupied--;

                    // Check if the current move is better
                    if (moveScore > bestScore)
                    {
                        bestScore = moveScore; // update best score
                        bestMoveI = i; // Track row of best move
                        bestMoveJ = j; // Track column of best move
                    }
                }
            }
        }

        if (bestMoveI != -1 && bestMoveJ != -1) // Mark the best move found
        {
            // Execute the best move found
            retCell.i=bestMoveI;
            retCell.j=bestMoveJ;
            retCell.value=O;
        }
    }

    return retCell;
}

#endif