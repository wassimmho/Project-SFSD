#define RAYGUI_IMPLEMENTATION
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <math.h>
#include <time.h>
#include "../include/raylib.h"
#include "../include/raygui.h"

int main(void)
{
    // Initialize the window with Raylib
    const int screenWidth = 1080;
    const int screenHeight = 920;
    InitWindow(screenWidth, screenHeight, "Raygui with Raylib");

    // Set the target FPS (frames per second)
    SetTargetFPS(60);

    // Define matrix parameters
    int rows = 5;  // Number of rows in the matrix
    int cols = 5;  // Number of columns in the matrix
    int blockSize = 100; // Size of each block (50x50)

    // Define color for text inside blocks
    // Define title color
    Color titleColor = GetColor(0xFF5733FF); // Equivalent to #FF5733
    Color textColor = GetColor(0x0092ff);
    Color boxColor = (Color){ 0xBA, 0xFF, 0x66, 0xFF }; 

    
    // Main game loop
    while (!WindowShouldClose())
    {
        // Update
        // (Add any game or UI logic here)

        // Begin drawing
        BeginDrawing();
        
        ClearBackground(RAYWHITE); // Clear the background

        // Drawing the title at the top
        DrawText("Matrix of Blocks", 400, 20, 20, titleColor);

        // Drawing the buttons (your existing code)
        if (GuiButton((Rectangle){0, 100, 75, 25}, "Initialize Secondary Memory"))
        {
            // Button clicked
            DrawText("Button clicked!", 300, 200, 20, DARKGREEN);
        }

        if (GuiButton((Rectangle){100, 100, 75, 25}, "Creat File"))
        {
            // Button clicked
            DrawText("Button clicked!", 300, 200, 20, BLUE);
        }

        if (GuiButton((Rectangle){200, 100, 75, 25}, "Rename File"))
        {
            // Button clicked
            DrawText("Button clicked!", 300, 200, 20, BLACK);
        }

        if (GuiButton((Rectangle){300, 100, 75, 25}, "Insert Record"))
        {
            // Button clicked
            DrawText("Button clicked!", 300, 200, 20, BLACK);
        }

        if (GuiButton((Rectangle){400, 100, 75, 25}, "Free MS"))
        {
            // Button clicked
            DrawText("Button clicked!", 300, 200, 20, BLACK);
        }  

        if (GuiButton((Rectangle){500, 100, 75, 25}, "Delete File"))
        {
            // Button clicked
            DrawText("Button clicked!", 300, 200, 20, BLACK);
        }

        if (GuiButton((Rectangle){600, 100, 75, 25}, "Delete Record"))
        {
            // Button clicked
            DrawText("Button clicked!", 300, 200, 20, BLACK);
        }

        if (GuiButton((Rectangle){700, 100, 75, 25}, "Exit program"))
        {
            // Button clicked
            DrawText("Button clicked!", 300, 200, 20, BLACK);
        }

        // Drawing the square matrix of lime-colored blocks with text inside
        for (int row = 0; row < rows; row++)
        {
            for (int col = 0; col < cols; col++)
            {
                // Calculate the position of each block
                int x = 300 + col * (blockSize + 20); // Positioning blocks with some space between them
                int y = 250 + row * (blockSize + 20);
                
                // Draw the lime-colored block (DrawRectangle(x, y, width, height, color))
                DrawRectangle(x, y, blockSize, blockSize, boxColor);

                // Draw the text inside the block (center the text)
                char blockText[10];
                snprintf(blockText, sizeof(blockText), "Block %d", row * cols + col + 1); // Example text for each block
                DrawText(blockText, x + blockSize / 4, y + blockSize / 4, 15, textColor); // Center the text in the block
            }
        }

        EndDrawing(); // End drawing
    }

    // De-initialization
    CloseWindow(); // Close window and OpenGL context

    return 0;
}
