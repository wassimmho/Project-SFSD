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


// Global variables

bool isButtonEnabled = false; // The boolean controlling button state

// Define matrix parameters
    int rows = 5;  // Number of rows in the matrix
    int cols = 5;  // Number of columns in the matrix
    int blockSize = 100; // Size of each block (50x50)
    int redCounter = 0; // Counter for free blocks
    int fileCounter = 0; // Counter for free blocks
    int interselectedMode = 0; // Selected mode (0 = Internal, 1 = External)
    int GlobselectedMode = 0; // Selected mode (0 = Internal, 1 = External)

    // Define colors
    Color titleColor; // Title color
    Color textColor; // Text color inside blocks
    Color boxColor = (Color){0xBA, 0xFF, 0x66, 0xFF}; // Block color
    // Draw the left frame with a different color for buttons
    Color leftFrameColor = (Color){0x4D, 0x4D, 0x4D, 0xFF}; // Dark grey for the frame
    char userInput[100] = ""; // Buffer to store user input
    char fileIDInput[100] = "";
    char NumberofBlocs[2] = "";
    char NumberofRecords[1] = "";

int main(void) {
    // Initialize the application window
    InitWindow(1080, 940, "File Management System");
    SetTargetFPS(60);  // Set the frame rate to 60 FPS

    // Initialize colors
    titleColor = GetColor(0xFF5733FF); // Title color
    textColor = GetColor(0x0092FF); // Text color inside blocks

    // Variables to manage application state
    int currentScreen = 0; // Tracks the active screen (0: Main Page, 1: Initialize Memory, etc.)

    // Buffers for text input
    char userInput[100] = "";  // Buffer for general input (e.g., file name)
    char fileIDInput[100] = "";  // Buffer for file ID input
    char recordInput[100] = "";  // Buffer for record input

    // Main application loop
    while (!WindowShouldClose()) {  // Check if the user wants to close the window
        BeginDrawing();
        ClearBackground(RAYWHITE);  // Set the background color

        // Draw the navigation menu on the left
        DrawRectangle(0, 0, 200, 920, LIGHTGRAY);
        if (isButtonEnabled)
        {
                // Matrix drawing below the title, within the bigger white section
                for (int row = 0; row < rows; row++) {
                    for (int col = 0; col < cols; col++) {
                        int x = 350 + col * (blockSize + 20); // Positioning blocks
                        int y = 200 + row * (blockSize + 20);

                        DrawRectangle(x, y, blockSize, blockSize, boxColor); // Block
                        char blockText[10];
                        snprintf(blockText, sizeof(blockText), "Block %d", row * cols + col + 1); // Block label
                        DrawText(blockText, x + blockSize / 4, y + blockSize / 4, 15, textColor); // Text
                    }
                }
        }
        


        // Navigation buttons
        if (GuiButton((Rectangle){20, 20, 160, 30}, "Main Page")) {
            currentScreen = 0;
        }
        if (GuiButton((Rectangle){20, 60, 160, 30}, "Initialize Memory")) {
            currentScreen = 1;
        }
        
        if (isButtonEnabled) {
            if (GuiButton((Rectangle){20, 100, 160, 30}, "Create File")) {
                currentScreen = 2;
            }
            if (GuiButton((Rectangle){20, 140, 160, 30}, "Rename File")) {
                currentScreen = 3;
            }
            if (GuiButton((Rectangle){20, 180, 160, 30}, "Insert Record")) {
                currentScreen = 4;
            }
            if (GuiButton((Rectangle){20, 220, 160, 30}, "Free Memory Space")) {
                currentScreen = 5;
            }
            if (GuiButton((Rectangle){20, 260, 160, 30}, "Delete File")) {
                currentScreen = 6;
            }
            if (GuiButton((Rectangle){20, 300, 160, 30}, "Delete Record")) {
                currentScreen = 7;
            }
        } else {
            GuiDisable();
            GuiButton((Rectangle){20, 100, 160, 30}, "Create File");
            GuiButton((Rectangle){20, 140, 160, 30}, "Rename File");
            GuiButton((Rectangle){20, 180, 160, 30}, "Insert Record");
            GuiButton((Rectangle){20, 220, 160, 30}, "Free Memory Space");
            GuiButton((Rectangle){20, 260, 160, 30}, "Delete File");
            GuiButton((Rectangle){20, 300, 160, 30}, "Delete Record");
            GuiEnable();
        }

        // Render the current screen based on the value of currentScreen
        switch (currentScreen) {
            case 0:
                // Main Page
                DrawText("Welcome to the File Management System", 220, 50, 20, DARKGRAY);
                break;
            case 1:
                // Initialize Memory Screen
                DrawText("Initialize Secondary Memory", 220, 50, 20, DARKGRAY);
                if (GuiButton((Rectangle){220, 100, 150, 30}, "Initialize")) {
                    redCounter++;  // Increment the counter for initialization calls
                    isButtonEnabled = 1;
                    TraceLog(LOG_INFO, "Memory initialized. Counter: %d", redCounter);
                }
                break;
            case 2:
                // Create File Screen
                DrawText("Create File", 220, 50, 20, DARKGRAY);
                GuiTextBox((Rectangle){220, 100, 200, 30}, userInput, sizeof(userInput), true);  // Input for file name
                GuiTextBox((Rectangle){220, 140, 200, 30}, fileIDInput, sizeof(fileIDInput), true);  // Input for file size
                if (GuiButton((Rectangle){220, 180, 150, 30}, "Create")) {
                    TraceLog(LOG_INFO, "File created: %s, Size: %d", userInput, atoi(fileIDInput));  // Log file creation
                }
                break;
            case 3:
                // Rename File Screen
                DrawText("Rename File", 220, 50, 20, DARKGRAY);
                GuiTextBox((Rectangle){220, 100, 200, 30}, userInput, sizeof(userInput), true);  // Input for old file name
                GuiTextBox((Rectangle){220, 140, 200, 30}, fileIDInput, sizeof(fileIDInput), true);  // Input for new file name
                if (GuiButton((Rectangle){220, 180, 150, 30}, "Rename")) {
                    TraceLog(LOG_INFO, "File renamed from %s to %s", userInput, fileIDInput);  // Log file rename
                }
                break;
            case 4:
                // Insert Record Screen
                DrawText("Insert Record", 220, 50, 20, DARKGRAY);
                GuiTextBox((Rectangle){220, 100, 200, 30}, fileIDInput, sizeof(fileIDInput), true);  // Input for file ID
                GuiTextBox((Rectangle){220, 140, 200, 30}, recordInput, sizeof(recordInput), true);  // Input for record data
                if (GuiButton((Rectangle){220, 180, 150, 30}, "Insert")) {
                    TraceLog(LOG_INFO, "Record inserted in file %s: %s", fileIDInput, recordInput);  // Log record insertion
                }
                break;
            case 5:
                // Free Memory Space Screen
                DrawText("Free Memory Space", 220, 50, 20, DARKGRAY);
                if (GuiButton((Rectangle){220, 100, 150, 30}, "Free Memory")) {
                    isButtonEnabled = 0;
                    TraceLog(LOG_INFO, "Memory space freed.");  // Log memory space freeing
                }

                
                break;
            case 6:
                // Delete File Screen
                DrawText("Delete File", 220, 50, 20, DARKGRAY);
                GuiTextBox((Rectangle){220, 100, 200, 30}, userInput, sizeof(userInput), true);  // Input for file name
                if (GuiButton((Rectangle){220, 140, 150, 30}, "Delete")) {
                    TraceLog(LOG_INFO, "File deleted: %s", userInput);  // Log file deletion
                }
                break;
            case 7:
                // Delete Record Screen
                DrawText("Delete Record", 220, 50, 20, DARKGRAY);
                GuiTextBox((Rectangle){220, 100, 200, 30}, fileIDInput, sizeof(fileIDInput), true);  // Input for file ID
                if (GuiButton((Rectangle){220, 140, 150, 30}, "Delete")) {
                    TraceLog(LOG_INFO, "Record deleted from file %s", fileIDInput);  // Log record deletion
                }
                break;
            default:
                // Error case for invalid screen value
                DrawText("Invalid screen", 220, 50, 20, RED);
                break;
        }

        EndDrawing();  // End rendering the current frame
    }

    CloseWindow();  // Close the application window
    return 0;
}