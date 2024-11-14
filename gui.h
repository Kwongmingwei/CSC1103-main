#include <stdlib.h>
#include "raylib.h"
#include "raymath.h"
#include "logic.h"
#include "ai.h"

#ifndef GUI_H
#define HEGUI_H
void CellDraw(Cell);
bool CellMark(int, int, CellValue);
bool GameIsOver(CellValue *);

void UpdateGamePage(void);

void DrawMainMenu() // create a menu page with 3 buttons: 1p mode & 2p mode & quit
{

    ClearBackground(BGCOLOR);

    // Title Text
    DrawText("Welcome to", screenWidth / 2 - MeasureText("Welcome to", 60) / 2, 85, 60, BLACK);
    DrawText("Tic-Tac-Toe!", screenWidth / 2 - MeasureText("Tic-Tac-Toe!", 60) / 2, 150, 60, BLACK);
    //1P Mode button
    Rectangle P1Button = {screenWidth / 2 - 100, 300, 250, 60};
    bool p1ButtonHover = CheckCollisionPointRec(GetMousePosition(), P1Button);
    DrawRectangleRec(P1Button, p1ButtonHover ? BUTTONCOLOR : BUTTONHOVER);
    DrawText("1P Mode", P1Button.x + 65, P1Button.y + 15, 30, BLACK);

    // 2P Mode Button
    Rectangle P2Button = {screenWidth / 2 - 100, 450, 250, 60};
    bool p2ButtonHover = CheckCollisionPointRec(GetMousePosition(), P2Button);
    DrawRectangleRec(P2Button, p2ButtonHover ? BUTTONCOLOR : BUTTONHOVER);
    DrawText("2P Mode", P2Button.x + 65, P2Button.y + 15, 30, BLACK);
    //Quit Button
    Rectangle ExitButton = {screenWidth / 2 - 100, 600, 250, 60};
    bool ExitButtonHover = CheckCollisionPointRec(GetMousePosition(), ExitButton);
    DrawRectangleRec(ExitButton, ExitButtonHover ? BUTTONCOLOR : BUTTONHOVER);
    DrawText("Quit", ExitButton.x + 85, ExitButton.y + 15, 30, BLACK);

    // Check button clicks
    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
    {

        if (p1ButtonHover)
        {
            mode = 0;
            screen = DIFFICULTY; // screen goes to difficulty page when user press on 1P mode
            waitForRelease = true;
        }
        if (p2ButtonHover)
        {
            mode = 1;
            screen = PLAY; 
            waitForRelease = true; // Set flag to wait for release
            PlaySound(Startup);
            GameInit(); // starts game
        }
        if (ExitButtonHover)
        {
            CloseWindow();
        }
    }
}

void DrawDiffMenu() // difficulty page with 4 buttons : easy, medium, hard, home
{
    difficulty = 0;
    ClearBackground(BGCOLOR);
    DrawText("Choose Difficulty!", screenWidth / 2 - MeasureText("Choose Difficulty!", 60) / 2, 150, 60, BLACK);

    Rectangle easyButton = {screenWidth / 2 - 100, 300, 200, 50};
    bool easyBHover = CheckCollisionPointRec(GetMousePosition(), easyButton);
    DrawRectangleRec(easyButton, easyBHover ? BUTTONCOLOR : BUTTONHOVER);
    DrawText("Easy", easyButton.x + 70, easyButton.y + 15, 20, BLACK);

    Rectangle medButton = {screenWidth / 2 - 100, 450, 200, 50};
    bool medBHover = CheckCollisionPointRec(GetMousePosition(), medButton);
    DrawRectangleRec(medButton, medBHover ? BUTTONCOLOR : BUTTONHOVER);
    DrawText("Medium", medButton.x + 70, medButton.y + 15, 20, BLACK);

    Rectangle hardButton = {screenWidth / 2 - 100, 600, 200, 50};
    bool hardBHover = CheckCollisionPointRec(GetMousePosition(), hardButton);
    DrawRectangleRec(hardButton, hardBHover ? BUTTONCOLOR : BUTTONHOVER);
    DrawText("Hard", hardButton.x + 70, hardButton.y + 15, 20, BLACK);

    Rectangle homeButton = {screenWidth / 2 - 50, 700, home.width * homeScale + 20, home.height * homeScale + 20};
    bool homeBHover = CheckCollisionPointRec(GetMousePosition(), homeButton);
    DrawRectangleRec(homeButton, homeBHover ? BUTTONCOLOR : BUTTONHOVER);
    DrawTextureEx(home, (Vector2){homeButton.x + 10, homeButton.y + 10}, 0.0, homeScale, YELLOW);

    // Check button clicks
    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
    {
        if (easyBHover) //Bayes
        {
            difficulty = 1;
            screen = PLAY;
            waitForRelease = true; // Set flag to wait for release
            PlaySound(Startup);
            GameInit();
            UpdateGamePage();
        }
        if (medBHover) //Imperfect Minimax
        {
            difficulty = 2;
            screen = PLAY;
            waitForRelease = true; // Set flag to wait for release
            PlaySound(Startup);
            GameInit();
            UpdateGamePage();
        }
        if (hardBHover) //Perfect Minimax
        {
            difficulty = 3;
            screen = PLAY;
            waitForRelease = true; // Set flag to wait for release
            PlaySound(Startup);
            GameInit();
            UpdateGamePage();
        }

        if (homeBHover)
        {
            screen = MENU;
        }
    }
}

void CellDraw(Cell cell)//Draw the X and O logo based on cell value
{
    Rectangle dest = {cell.i * cellWidth, cell.j * cellHeight, cellWidth, cellHeight};

    Rectangle source;
    switch (cell.value)
    {
    case X:
        source = (Rectangle){0, 0, 225, 225};
        DrawTexturePro(x_logo, source, dest, (Vector2){0, 0}, 0, PINK); // load the X png
        break;
    case O:
        source = (Rectangle){0, 0, 225, 225};
        DrawTexturePro(o_logo, source, dest, (Vector2){0, 0}, 0, PINK); // load the O png
        break;
    default:
        break;
    }
    DrawRectangleLinesEx((Rectangle){cell.i * cellWidth, cell.j * cellHeight, cellWidth, cellHeight}, 3, BLACK);
}

bool CellMark(int i, int j, CellValue value)
{
    if (grid[i][j].value != EMPTY || value == EMPTY) // check if cell is not empty or empty
    {
        return false; // the cell cannot be marked if it is falsed
    }
    PlaySound(Turn);
    grid[i][j].value = value; //Set value in the grid
    tilesOccupied++;

    return true;
}

void UpdateGamePage(void) //Show TicTacToe page
{

    if (mode == 0 && state == PLAYERO)//If it is 1P mode and AI turn
    {
        Cell tempCell = AITurn(); //Get cell where AI chose
        CellMark(tempCell.i, tempCell.j, tempCell.value); //Set Value in the board
        if (!GameIsOver(&winner))
        {
            if (tilesOccupied == 9) // Check for a tie when all tiles are occupied
            {
                state = END; //Game has ended
                winner = EMPTY;  // No winner in case of a tie
                PlaySound(Draw); // Play draw sound
                tieScore++;      // Increment tie score
            }
            else
            {
                state = (state == PLAYERX) ? PLAYERO : PLAYERX;
            }
        }
        else
        {
            state = END; //Game has ended
            PlaySound(Winner); // Play winner sound
            // Increment scores based on the winner
            if (winner == X)
                player1Score++; // player 1 score will increase by 1 if wins
            else if (winner == O)
                player2Score++; // player 2 score will increase by 1 if wins
        }
    }
    else
    {
        if (waitForRelease)
        {
            if (!IsMouseButtonDown(MOUSE_BUTTON_LEFT))
            {
                waitForRelease = false; // Clear flag once mouse is released
            }
        }
        else
        {
            // Normal gameplay click handling
            if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT))
            {
                Vector2 mPos = GetMousePosition();
                int indexI = mPos.x / cellWidth;
                int indexJ = mPos.y / cellHeight;
                if (state != END && IndexIsValid(indexI, indexJ))
                {
                    if (CellMark(indexI, indexJ, state == PLAYERX ? X : O))
                    {
                        if (!GameIsOver(&winner))
                        {
                            if (tilesOccupied == 9) // Check for a tie when all tiles are occupied
                            {
                                state = END; //Game has ended
                                winner = EMPTY;  // No winner in case of a tie
                                PlaySound(Draw); // Play draw sound
                                tieScore++;      // Increment tie score
                            }
                            else
                            {
                                state = (state == PLAYERX) ? PLAYERO : PLAYERX;
                            }
                        }
                        else
                        {
                            state = END; //Game has ended
                            PlaySound(Winner); // Play winner sound
                            // Increment scores based on the winner
                            if (winner == X)
                                player1Score++; // player 1 score will increase by 1 if wins
                            else if (winner == O)
                                player2Score++; // player 2 score will increase by 1 if wins
                        }
                    }
                }
            }

            if (state == END) //
            {
                if (IsKeyPressed(KEY_R))
                {
                    GameInit(); // pressing R button starts a new game
                }
                
                else if (IsKeyPressed(KEY_ESCAPE))// Press ESC to return to the menu
                {
                    screen = MENU; 
                    waitForRelease = true;
                    player1Score = 0;
                    player2Score = 0;
                    tieScore = 0;
                }
            }

            ClearBackground(BGCOLOR);

            // Draw the grid and cells
            for (int i = 0; i < COLS; i++)
            {
                for (int j = 0; j < ROWS; j++)
                {
                    CellDraw(grid[i][j]);
                }
            }

            // Display score at the top of the screen
            DrawText(TextFormat("P1: %d  T: %d  P2: %d", player1Score, tieScore, player2Score), 200, 900, 60, DARKGRAY);

            Rectangle homeBButton = {20, 880, home.width * homeScale + 20, home.height * homeScale + 20};
            bool homeBBHover = CheckCollisionPointRec(GetMousePosition(), homeBButton);
            DrawRectangleRec(homeBButton, homeBBHover ? PINK : BGCOLOR);
            DrawTextureEx(home, (Vector2){homeBButton.x + 10, homeBButton.y + 10}, 0.0, homeScale, BGCOLOR);

            // If the button is clicked, go back to the main menu
            if (homeBBHover && IsMouseButtonReleased(MOUSE_BUTTON_LEFT))
            {
                player1Score = 0;
                player2Score = 0;
                tieScore = 0;
                screen = MENU;
                GameInit();
            }

            if (state == END)
            {
                const char *text;
                if (winner == EMPTY && tilesOccupied == 9) // If no winner and 9 tiles filled up
                {
                    text = gameTied; // print it's a tie
                }
                else
                {
                    text = (winner == X) ? player1Wins : player2Wins;
                }

                DrawRectangle(0, 0, screenWidth, screenHeight, Fade(WHITE, 0.8f));
                DrawText(text, screenWidth / 2 - MeasureText(text, 40) / 2, screenHeight / 2 - 10, 40, DARKGRAY);
                DrawText(pressRToRestart, screenWidth / 2 - MeasureText(pressRToRestart, 20) / 2, screenHeight * 0.75f - 10, 20, DARKGRAY);

                DrawText("Press 'ESC' to return to menu.", screenWidth / 2 - MeasureText("Press 'ESC' to return to menu.", 20) / 2, screenHeight * 0.75f + 30, 20, DARKGRAY);
            }
            else
            {
                const char *text = (state == PLAYERX) ? player1TurnPrompt : player2TurnPrompt;

                Vector2 messagePosition = (Vector2){screenWidth / 2, 40};

                int measure = MeasureText(text, 40);
                Rectangle textRect = (Rectangle){messagePosition.x - measure / 2 - 40, messagePosition.y - 40, measure + 80, 80};

                DrawRectangleRec(textRect, Fade(WHITE, 0.8f));
                DrawText(text, messagePosition.x - measure / 2, messagePosition.y - 20, 40, DARKGRAY);
            }
        }
    }
}
#endif
