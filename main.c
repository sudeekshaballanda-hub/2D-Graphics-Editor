#include <stdio.h>
#include <stdlib.h>

// Global variables for dynamic sizing
int ROWS;
int COLS;
char** canvas = NULL; // A pointer-to-pointers for our dynamic 2D character array

// Function to allocate memory and initialize the canvas with underscores
void initializeCanvas() {
    // 1. Allocate memory for the row pointers
    canvas = (char**)malloc(ROWS * sizeof(char*));
    
    // 2. Allocate memory for columns in each row and fill with '_'
    for (int i = 0; i < ROWS; i++) {
        canvas[i] = (char*)malloc(COLS * sizeof(char));
        for (int j = 0; j < COLS; j++) {
            canvas[i][j] = '_';
        }
    }
}

// Function to print the canvas to the screen
void displayCanvas() {
    // Clears the console window based on OS
    #ifdef _WIN32
        system("cls");   // Windows
    #else
        system("clear"); // Linux / macOS
    #endif
    
    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COLS; j++) {
            printf("%c ", canvas[i][j]); // Print character followed by a space
        }
        printf("\n"); // Move to the next line after finishing a row
    }
}

// Memory cleanup function to avoid memory leaks (Crucial for Advanced C!)
void freeCanvas() {
    if (canvas != NULL) {
        for (int i = 0; i < ROWS; i++) {
            free(canvas[i]); // Free individual rows
        }
        free(canvas); // Free the main pointer array
    }
}

int main() {
    // Take dynamic dimension inputs from user
    printf("Enter the number of rows for your canvas: ");
    scanf("%d", &ROWS);
    printf("Enter the number of columns for your canvas: ");
    scanf("%d", &COLS);

    // Build the canvas
    initializeCanvas();

    int choice = 0;
    while (choice != 5) {
        printf("\n--- 2D Graphics Editor (Pure C) ---\n");
        printf("1. Add Object\n");
        printf("2. Delete Object\n");
        printf("3. Modify Object\n");
        printf("4. Display Picture\n");
        printf("5. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch(choice) {
            case 1:
                printf("Adding object... (Logic coming on Day 2)\n");
                break;
            case 2:
                printf("Deleting object... (Logic coming on Day 2)\n");
                break;
            case 3:
                printf("Modifying object... (Logic coming on Day 2)\n");
                break;
            case 4:
                displayCanvas();
                break;
            case 5:
                printf("Exiting program. Goodbye!\n");
                break;
            default:
                printf("Invalid choice! Try again.\n");
        }
    }

    // Clean up dynamic memory before exiting
    freeCanvas();
    return 0;
}