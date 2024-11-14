// Raylib Tic-Tac-Toe
#include <stdlib.h>
#include "raylib.h"
#include "raymath.h"
#include "gui.h"

int main()
{
    //Loading off data from file named "tic-tac-toe.data"
    load_data(table_for_board, table_for_outcome); //Save in arrays called table-for-board and table_for outcome
    split_data(table_for_board,table_for_outcome,board_train,outcome_train, board_test,outcome_test,max_rows); // Split data into training and testing set
    trainNaiveBayes(board_train, outcome_train, (int) max_rows * (1 - 0.2), class_count, likelihood); // Training of bayes model

    InitWindow(screenWidth, screenHeight, "Tic-Tac-Toe Game"); // create window with header
    SetExitKey(0); //Disable Exit Key

    InitAudioDevice(); // // Must be done before playing any sounds

    o_logo = LoadTexture("resources/o.png"); // load png from the resources folder
    x_logo = LoadTexture("resources/x.png");
    home = LoadTexture("resources/home.png");

    Startup = LoadSound("resources/Startup.wav"); // Load sounds from the resources folder
    Turn = LoadSound("resources/Turn.wav");
    Draw = LoadSound("resources/draw.wav");
    Winner = LoadSound("resources/winner.wav");

    while (!WindowShouldClose())
    {
        BeginDrawing();
        switch (screen) // Switch between game screens based on the current state of `screen`
        {
        case MENU: // Main menu screen
            DrawMainMenu();
            break;
        case PLAY: // Gameplay screen
            UpdateGamePage();
            break;
        case DIFFICULTY: // Difficulty selection screen
            DrawDiffMenu();
            break;
        }
        EndDrawing();
    }

    // Unload sound and logos before closing window to avoid memory leaks
    UnloadTexture(o_logo);
    UnloadTexture(x_logo);
    UnloadTexture(home);
    UnloadSound(Startup);
    UnloadSound(Turn);
    UnloadSound(Draw);
    UnloadSound(Winner);
    CloseAudioDevice();

    CloseWindow();

    return 0; // End of the program
}