#define RAYGUI_IMPLEMENTATION
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <math.h>
#include <time.h>
#include "../include/raylib.h"
#include "../include/raygui.h"
#include "../include/header.h"

int main(void) {
    // Initialize the window with Raylib
    const int screenWidth = 1080;
    const int screenHeight = 920;
    InitWindow(screenWidth, screenHeight, "FILE MANAGEMENT SYSTEM");

    // Set the target FPS (frames per second)
    SetTargetFPS(60);

    // Define matrix parameters
    int rows = 5;  // Number of rows in the matrix
    int cols = 5;  // Number of columns in the matrix
    int blockSize = 100; // Size of each block (50x50)
    int redCounter = 0; // Counter for free blocks
    int fileCounter = 0; // Counter for free blocks
    int interselectedMode = 0; // Selected mode (0 = Internal, 1 = External)
    int GlobselectedMode = 0; // Selected mode (0 = Internal, 1 = External)

    // Define colors
    Color titleColor = GetColor(0xFF5733FF); // Title color
    Color textColor = GetColor(0x0092FF); // Text color inside blocks
    Color boxColor = (Color){0xBA, 0xFF, 0x66, 0xFF}; // Block color
    // Draw the left frame with a different color for buttons
    Color leftFrameColor = (Color){0x4D, 0x4D, 0x4D, 0xFF}; // Dark grey for the frame
    char userInput[100] = ""; // Buffer to store user input
    char fileIDInput[100] = "";
    char NumberofBlocs[2] = "";
    char NumberofRecords[1] = "";
    const char *globalOptions[] = {"Contiguous", "Chained"};  // Global options (Contiguous or Chained)
    const char *internalOptions[] = {"Sorted", "Unsorted"};   // Internal options (Sorted or Unsorted)

    // Track the current screen
    int currentScreen = -1; 

    // Main game loop
    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(RAYWHITE); // Clear the background

        
        
        // Use a switch statement for navigation
        switch (currentScreen) {


             case -1: { // Second Page

                DrawText(TextFormat("Welcom to your file management system"), 500, 20, 20, RED);
                if (GuiButton((Rectangle){400, 300, 150, 50}, "Create MS")) {
                    currentScreen = -2;
                }
                
                
                break;
            }

            case -2: { // Second Page

                DrawText(TextFormat("Initialize MS"), 400, 50, 20, RED);
                if (GuiButton((Rectangle){480, 300, 150, 50}, "Create MS")) {
                    currentScreen = 0;
                }
                
                
                break;
            }
    


            case 0: { // Main Page
                
                
                          
                DrawRectangle(0, 0, 250, screenHeight, leftFrameColor); // Left frame
                
                
                

                // Drawing the buttons in the left frame
                if (GuiButton((Rectangle){50, 100, 150, 50}, "Initialize Secondary Memory")) {
                    currentScreen = 1;
                }

                if (GuiButton((Rectangle){50, 180, 150, 50}, "Create File")) {
                    currentScreen = 2;
                }

                if (GuiButton((Rectangle){50, 260, 150, 50}, "Rename File")) {
                    currentScreen = 3;
                }

                if (GuiButton((Rectangle){50, 340, 150, 50}, "Insert Record")) {
                    currentScreen = 4;
                }

                if (GuiButton((Rectangle){50, 420, 150, 50}, "Free MS")) {
                    currentScreen = 5;
                }

                if (GuiButton((Rectangle){50, 500, 150, 50}, "Delete File")) {
                    currentScreen = 6;
                }

                if (GuiButton((Rectangle){50, 580, 150, 50}, "Delete Record")) {
                    currentScreen = 7;
                }

                // Displaying information on the bigger section
                DrawText(TextFormat("Allocated memory blocks: %d", redCounter), 50, 800, 15, WHITE);
                DrawText(TextFormat("Number of files: %d", fileCounter), 50, 830, 15, WHITE);

                // White background for the bigger section (Right part of the screen)
                DrawRectangle(250, 0, screenWidth - 250, screenHeight, RAYWHITE);
                // Title
                // Title at the top of the bigger section (Centered)
                DrawText("Main Page: Secondary Memory State", 400 , 50, 25, RED);

                // Matrix drawing below the title, within the bigger white section
                for (int row = 0; row < rows; row++) {
                    for (int col = 0; col < cols; col++) {
                        int x = 350 + col * (blockSize + 20); // Positioning blocks
                        int y = 150 + row * (blockSize + 20);

                        DrawRectangle(x, y, blockSize, blockSize, boxColor); // Block
                        char blockText[10];
                        snprintf(blockText, sizeof(blockText), "Block %d", row * cols + col + 1); // Block label
                        DrawText(blockText, x + blockSize / 4, y + blockSize / 4, 15, textColor); // Text
                    }
                }


                



                break;
            }
            case 1: { // Second Page

                // Draw Left frame
                DrawRectangle(0, 0, 250, screenHeight, leftFrameColor); // Left frame
                DrawText("Rename File", 450, 30, 25, titleColor);

                // User Input for New File Name
                DrawText("Provide Number of blocs in MS:", 400, 200, 20, DARKGRAY);
                GuiTextBox((Rectangle){400, 230, 300, 30}, NumberofBlocs, sizeof(NumberofBlocs), true);

                // User Input for File ID
                DrawText("Enter Number of Records in a Bloc:", 400, 270, 20, DARKGRAY);
                GuiTextBox((Rectangle){400, 300, 300, 30}, NumberofRecords, sizeof(NumberofRecords), true);

                // Title
                // DrawText("Select the storage mode", 300, 100, 20, DARKGRAY);

                // Title
                // DrawText("Select Global Organization mode", 300, 100, 20, DARKGRAY);

                // Dropdown menu for choosing the mode
                // GlobselectedMode = GuiComboBox((Rectangle){300, 200, 200, 30}, "Select Mode", globalOptions);

                // Title
                // DrawText("Select Internal Organization mode", 300, 100, 20, DARKGRAY);

                // Dropdown menu for choosing the mode
                // interselectedMode = GuiComboBox((Rectangle){300, 400, 200, 30}, "Select Mode", internalOptions);


                // Buttons for navigation
                if (GuiButton((Rectangle){50, 500, 150, 50}, "Go Back")) {
                    currentScreen = 0; // Go back to the main page
                }

                if (GuiButton((Rectangle){50, 600, 150, 50}, "Exit Program")) {
                    CloseWindow();
                    return 0;
                }

                break;
            }


            case 2: { // Create File
                DrawRectangle(0, 0, 250, screenHeight, leftFrameColor); // Left frame
                DrawText("Create File", 450, 30, 25, titleColor);

                // User Input for New File Name
                DrawText("Provide File's Name:", 400, 200, 20, DARKGRAY);
                GuiTextBox((Rectangle){400, 230, 300, 30}, userInput, sizeof(userInput), true);

                // User Input for File ID
                DrawText("Size of the file in blocs:", 400, 270, 20, DARKGRAY);
                GuiTextBox((Rectangle){400, 300, 300, 30}, fileIDInput, sizeof(fileIDInput), true);

                // Centered Buttons for Rename File Page
                if (GuiButton((Rectangle){50, 500, 150, 50}, "Go Back")) {
                    currentScreen = 0;
                }

                if (GuiButton((Rectangle){50, 600, 150, 50}, "Exit Program")) {
                    CloseWindow();
                    return 0;
                }

                break;
            }

            case 3: { // Insert Record
                DrawRectangle(0, 0, 250, screenHeight, leftFrameColor); // Left frame
                DrawText("Rename File", 450, 30, 25, titleColor);

                // User Input for New File Name
                DrawText("Provide File's New Name:", 400, 200, 20, DARKGRAY);
                GuiTextBox((Rectangle){400, 230, 300, 30}, userInput, sizeof(userInput), true);

                // User Input for File ID
                DrawText("Enter File ID:", 400, 270, 20, DARKGRAY);
                GuiTextBox((Rectangle){400, 300, 300, 30}, fileIDInput, sizeof(fileIDInput), true);

                // Centered Buttons for Rename File Page
                if (GuiButton((Rectangle){50, 500, 150, 50}, "Go Back")) {
                    currentScreen = 0;
                }

                if (GuiButton((Rectangle){50, 600, 150, 50}, "Exit Program")) {
                    CloseWindow();
                    return 0;
                }

                break;
            }

            case 4: { // Second Page
                DrawRectangle(0, 0, 250, screenHeight, leftFrameColor); // Left frame
                DrawText("Rename File", 450, 30, 25, titleColor);

                // User Input for New File Name
                DrawText("Provide File's New Name:", 400, 200, 20, DARKGRAY);
                GuiTextBox((Rectangle){400, 230, 300, 30}, userInput, sizeof(userInput), true);

                // User Input for File ID
                DrawText("Enter File ID:", 400, 270, 20, DARKGRAY);
                GuiTextBox((Rectangle){400, 300, 300, 30}, fileIDInput, sizeof(fileIDInput), true);

                // Centered Buttons for Rename File Page
                if (GuiButton((Rectangle){50, 500, 150, 50}, "Go Back")) {
                    currentScreen = 0;
                }

                if (GuiButton((Rectangle){50, 600, 150, 50}, "Exit Program")) {
                    CloseWindow();
                    return 0;
                }

                break;
            }

            case 5: { // Second Page
                DrawRectangle(0, 0, 250, screenHeight, leftFrameColor); // Left frame
                DrawText("Rename File", 450, 30, 25, titleColor);

                // User Input for New File Name
                DrawText("Provide File's New Name:", 400, 200, 20, DARKGRAY);
                GuiTextBox((Rectangle){400, 230, 300, 30}, userInput, sizeof(userInput), true);

                // User Input for File ID
                DrawText("Enter File ID:", 400, 270, 20, DARKGRAY);
                GuiTextBox((Rectangle){400, 300, 300, 30}, fileIDInput, sizeof(fileIDInput), true);

                // Centered Buttons for Rename File Page
                if (GuiButton((Rectangle){50, 500, 150, 50}, "Go Back")) {
                    currentScreen = 0;
                }

                if (GuiButton((Rectangle){50, 600, 150, 50}, "Exit Program")) {
                    CloseWindow();
                    return 0;
                }

                break;
            }

            case 6: { // Second Page
                DrawRectangle(0, 0, 250, screenHeight, leftFrameColor); // Left frame
                DrawText("Rename File", 450, 30, 25, titleColor);

                // User Input for New File Name
                DrawText("Provide File's New Name:", 400, 200, 20, DARKGRAY);
                GuiTextBox((Rectangle){400, 230, 300, 30}, userInput, sizeof(userInput), true);

                // User Input for File ID
                DrawText("Enter File ID:", 400, 270, 20, DARKGRAY);
                GuiTextBox((Rectangle){400, 300, 300, 30}, fileIDInput, sizeof(fileIDInput), true);

                // Centered Buttons for Rename File Page
                if (GuiButton((Rectangle){50, 500, 150, 50}, "Go Back")) {
                    currentScreen = 0;
                }

                if (GuiButton((Rectangle){50, 600, 150, 50}, "Exit Program")) {
                    CloseWindow();
                    return 0;
                }

                break;
            }

            case 7: { // Second Page
                DrawRectangle(0, 0, 250, screenHeight, leftFrameColor); // Left frame
                DrawText("Rename File", 450, 30, 25, titleColor);

                // User Input for New File Name
                DrawText("Provide File's New Name:", 400, 200, 20, DARKGRAY);
                GuiTextBox((Rectangle){400, 230, 300, 30}, userInput, sizeof(userInput), true);

                // User Input for File ID
                DrawText("Enter File ID:", 400, 270, 20, DARKGRAY);
                GuiTextBox((Rectangle){400, 300, 300, 30}, fileIDInput, sizeof(fileIDInput), true);

                // Centered Buttons for Rename File Page
                if (GuiButton((Rectangle){50, 500, 150, 50}, "Go Back")) {
                    currentScreen = 0;
                }

                if (GuiButton((Rectangle){50, 600, 150, 50}, "Exit Program")) {
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
