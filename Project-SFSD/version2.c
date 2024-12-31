#define RAYGUI_IMPLEMENTATION
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <math.h>
#include <time.h>
#include "../include/raylib.h"
#include "../include/raygui.h"

int main(void) {
    // Initialize the window with Raylib
    const int screenWidth = 1080;
    const int screenHeight = 920;
    InitWindow(screenWidth, screenHeight, "Raygui with Navigation");

    // Set the target FPS (frames per second)
    SetTargetFPS(60);

    // Define matrix parameters
    int rows = 5;  // Number of rows in the matrix
    int cols = 5;  // Number of columns in the matrix
    int blockSize = 100; // Size of each block (50x50)
    int redCounter = 0; // Counter for free blocks
    // Define colors
    Color titleColor = GetColor(0xFF5733FF); // Title color
    Color textColor = GetColor(0x0092FF); // Text color inside blocks
    Color boxColor = (Color){0xBA, 0xFF, 0x66, 0xFF}; // Block color
    char userInput[100] = ""; // Buffer to store user input

    // Track the current screen
    int currentScreen = 0; // 0 = Main Page, 1 = Second Page

    // Main game loop
    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(RAYWHITE); // Clear the background

        // Use a switch statement for navigation
        switch (currentScreen) {
            case 0: { // Main Page
                DrawText("Main Page: Secondary Memory State", 400, 20, 20, titleColor);

                // Drawing the buttons
                if (GuiButton((Rectangle){50, 100, 150, 50}, "Initialize Secondary Memory")) {
                    currentScreen = 1;
                }

                if (GuiButton((Rectangle){250, 100, 100, 50}, "Create File")) {
                    currentScreen = 2;
                }

                if (GuiButton((Rectangle){400, 100, 100, 50}, "Rename File")) {
                    currentScreen = 3;
                }

                if (GuiButton((Rectangle){550, 100, 100, 50}, "Insert Record")) {
                    currentScreen = 4;
                }

                if (GuiButton((Rectangle){700, 100, 100, 50}, "Free MS")) {
                    currentScreen = 5;
                }

                if (GuiButton((Rectangle){850, 100, 100, 50}, "Delete File")) {
                    currentScreen = 6;
                }

                if (GuiButton((Rectangle){1000, 100, 100, 50}, "Delete Record")) {
                    currentScreen = 7;
                }

                DrawText(TextFormat("allocated memory blocs: %d", redCounter), 50, 200, 15, RED);                

                // Draw the square matrix of lime-colored blocks with text inside
                for (int row = 0; row < rows; row++) {
                    for (int col = 0; col < cols; col++) {
                        int x = 300 + col * (blockSize + 20); // Positioning blocks
                        int y = 250 + row * (blockSize + 20);

                        DrawRectangle(x, y, blockSize, blockSize, boxColor); // Block
                        char blockText[10];
                        snprintf(blockText, sizeof(blockText), "Block %d", row * cols + col + 1); // Block label
                        DrawText(blockText, x + blockSize / 4, y + blockSize / 4, 15, textColor); // Text
                    }
                }

                break;
            }
            case 1: { // Second Page
                DrawText("Second Page: Additional Options", 400, 20, 20, titleColor);

                GuiTextBox((Rectangle){200, 200, 300, 30}, userInput, sizeof(userInput), true); // User input text box

                if (GuiButton((Rectangle){200, 700, 150, 50}, "Go Back")) {
                    currentScreen = 0; // Go back to the main page
                }

                if (GuiButton((Rectangle){500, 700, 150, 50}, "Exit Program")) {
                    CloseWindow();
                    return 0;
                }

                break;
            }

            case 2: { // Second Page
                DrawText("Second Page: Additional Options", 400, 20, 20, titleColor);

                GuiTextBox((Rectangle){200, 200, 300, 30}, userInput, sizeof(userInput), true); // User input text box

                if (GuiButton((Rectangle){200, 700, 150, 50}, "Go Back")) {
                    currentScreen = 0; // Go back to the main page
                }

                if (GuiButton((Rectangle){500, 700, 150, 50}, "Exit Program")) {
                    CloseWindow();
                    return 0;
                }

                break;
            }


            case 3: { // Second Page
                DrawText("Second Page: Additional Options", 400, 20, 20, titleColor);

                GuiTextBox((Rectangle){200, 200, 300, 30}, userInput, sizeof(userInput), true); // User input text box

                if (GuiButton((Rectangle){200, 700, 150, 50}, "Go Back")) {
                    currentScreen = 0; // Go back to the main page
                }

                if (GuiButton((Rectangle){500, 700, 150, 50}, "Exit Program")) {
                    CloseWindow();
                    return 0;
                }

                break;
            }

            case 4: { // Second Page
                DrawText("Second Page: Additional Options", 400, 20, 20, titleColor);

                GuiTextBox((Rectangle){200, 200, 300, 30}, userInput, sizeof(userInput), true); // User input text box

                if (GuiButton((Rectangle){200, 700, 150, 50}, "Go Back")) {
                    currentScreen = 0; // Go back to the main page
                }

                if (GuiButton((Rectangle){500, 700, 150, 50}, "Exit Program")) {
                    CloseWindow();
                    return 0;
                }

                break;
            }


            case 5: { // Second Page
                DrawText("Second Page: Additional Options", 400, 20, 20, titleColor);

                GuiTextBox((Rectangle){200, 200, 300, 30}, userInput, sizeof(userInput), true); // User input text box

                if (GuiButton((Rectangle){200, 700, 150, 50}, "Go Back")) {
                    currentScreen = 0; // Go back to the main page
                }

                if (GuiButton((Rectangle){500, 700, 150, 50}, "Exit Program")) {
                    CloseWindow();
                    return 0;
                }

                break;
            }


            case 6: { // Second Page
                DrawText("Second Page: Additional Options", 400, 20, 20, titleColor);

                GuiTextBox((Rectangle){200, 200, 300, 30}, userInput, sizeof(userInput), true); // User input text box

                if (GuiButton((Rectangle){200, 700, 150, 50}, "Go Back")) {
                    currentScreen = 0; // Go back to the main page
                }

                if (GuiButton((Rectangle){500, 700, 150, 50}, "Exit Program")) {
                    CloseWindow();
                    return 0;
                }

                break;
            }

            case 7: { // Second Page
                DrawText("Second Page: Additional Options", 400, 20, 20, titleColor);

                GuiTextBox((Rectangle){200, 200, 300, 30}, userInput, sizeof(userInput), true); // User input text box

                if (GuiButton((Rectangle){200, 700, 150, 50}, "Go Back")) {
                    currentScreen = 0; // Go back to the main page
                }

                if (GuiButton((Rectangle){500, 700, 150, 50}, "Exit Program")) {
                    CloseWindow();
                    return 0;
                }

                break;
            }
        }
        

        EndDrawing(); // End drawing
    }

    // De-initialization
    CloseWindow(); // Close window and OpenGL context
    return 0;
}
