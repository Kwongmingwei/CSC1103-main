#include <stdlib.h>
#include "raylib.h"
#include "raymath.h"

#ifndef LOGIC_H //Good practice
#define LOGIC_H

#define COLS 3 // Number of columns
#define ROWS 3 // Number of rows

#define BGCOLOR SKYBLUE
#define BUTTONCOLOR ORANGE
#define BUTTONHOVER YELLOW

const int screenWidth = 800;
const int screenHeight = 1000;
const Vector2 screenSize = {screenWidth, screenHeight};
const float homeScale = 0.3;

const int cellWidth = screenWidth / COLS;
const int cellHeight = (screenHeight - 200) / ROWS;

const char *player1Wins = "PLAYER 1 WINS!"; // text should not be modified, pointer points to a memory location where the string is stored.
const char *player2Wins = "PLAYER 2 WINS!";
const char *pressRToRestart = "Press 'R' to play again!";
const char *player1TurnPrompt = "PLAYER 1 GO";
const char *player2TurnPrompt = "PLAYER 2 GO";
const char *gameTied = "IT'S A TIE!";


int player1Score = 0;
int player2Score = 0;
int tieScore = 0;
int mode = 1; //0 represent 1P mode, 1 represent 2P mode
int difficulty = 0;
int tilesOccupied; // Tracks the number of tiles occupied in the grid

typedef enum CellValue // Enum to represent the value of a cell (empty, X, or O)
{
    EMPTY,
    X,
    O
} CellValue;

typedef struct Cell // Structure for a cell in the grid
{
    int i;           // Row index of the cell
    int j;           // Column index of the cell
    CellValue value; //(EMPTY, X, or O)
} Cell;

Cell grid[COLS][ROWS];

typedef enum GameState // define different game state
{
    PLAYERX, // PLAYER X's turn
    PLAYERO, // PLAYER O's turn
    END      // State where the game is finished
} GameState;

typedef enum GameScreen // define different game screen pages (total 3)
{
    MENU,       // Main menu screen
    DIFFICULTY, // Difficulty selection screen
    PLAY        // Gameplay screen
} GameScreen;

GameState state;
CellValue winner;
Texture2D o_logo, x_logo, home; // declare to load texture

bool IndexIsValid(int, int);
void GridInit(void);
void GameInit(void);
bool GameIsOver(CellValue *); // Check if the game is over and determine the winner


Sound Startup, Turn, Draw, Winner;
GameScreen screen = MENU;
bool waitForRelease;

bool IndexIsValid(int i, int j) //Check the cols and rows are within defined range (3)
{
    return i >= 0 && i < COLS && j >= 0 && j < ROWS;
}

void GridInit(void) // Initialize the grid with all cells set to EMPTY
{
    for (int i = 0; i < COLS; i++) // Loop through columns
    {
        for (int j = 0; j < ROWS; j++) // Loop through rows
        {
            grid[i][j] = (Cell){// Set each cell's properties
                                .i = i,
                                .j = j,
                                .value = EMPTY}; // Set all cell value as empty
        }
    }
}

void GameInit(void) // Initialize the game state and grid
{
    GridInit();        // Reset the grid to an empty state
    state = PLAYERX;   // player 1 will always start as X
    tilesOccupied = 0; // Reset the count of occupied tiles
}

bool GameIsOver(CellValue *winner) //Check if there is a winner
{   //Check diagonal
    if ((grid[0][0].value == grid[1][1].value && grid[1][1].value == grid[2][2].value && grid[0][0].value != EMPTY) || (grid[0][2].value == grid[1][1].value && grid[1][1].value == grid[2][0].value && grid[0][2].value != EMPTY))
    {
        *winner = grid[1][1].value;
        return true;
    }

    for (int i = 0; i < 3; i++)
    {
        //Check horizontal rows
        if (grid[i][0].value == grid[i][1].value && grid[i][1].value == grid[i][2].value && grid[i][0].value != EMPTY)
        {
            *winner = grid[i][0].value;
            return true;
        }
        //Check vertical
        if (grid[0][i].value == grid[1][i].value && grid[1][i].value == grid[2][i].value && grid[0][i].value != EMPTY)
        {
            *winner = grid[0][i].value;
            return true;
        }
    }
    //No winner yet
    *winner = EMPTY;
    return false;
}

#endif