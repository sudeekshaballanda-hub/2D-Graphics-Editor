#include <stdio.h>
#include <stdlib.h>

// Global Canvas variables
int ROWS;
int COLS;
char** canvas = NULL;

// --- DAY 2: SHAPE TRACKING STRUCTURES ---
typedef enum { RECTANGLE = 1, LINE, CIRCLE, TRIANGLE } ShapeType;

typedef struct {
    int id;            // Unique ID to find, modify, or delete the shape
    ShapeType type;    // 1 = Rectangle, 2 = Line, etc.
    int x1, y1;        // Top-left corner for rectangle / Starting point
    int x2, y2;        // Bottom-right corner for rectangle / Ending point
} Shape;

Shape database[100];   // Array can hold up to 100 shapes
int shapeCount = 0;    // Keeps track of total active shapes
int nextId = 1;        // Automatically increments to assign unique IDs

// Core Canvas Functions
void initializeCanvas() {
    canvas = (char**)malloc(ROWS * sizeof(char*));
    for (int i = 0; i < ROWS; i++) {
        canvas[i] = (char*)malloc(COLS * sizeof(char));
    }
}

void clearCanvas() {
    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COLS; j++) {
            canvas[i][j] = '_';
        }
    }
}

// Drawing Algorithms
void drawRectangle(Shape rect) {
    // Loop through the canvas and draw the borders of the rectangle
    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COLS; j++) {
            // Check if current coordinate sits on the top, bottom, left, or right edge
            if ((i == rect.y1 || i == rect.y2) && (j >= rect.x1 && j <= rect.x2)) {
                canvas[i][j] = '*';
            }
            if ((j == rect.x1 || j == rect.x2) && (i >= rect.y1 && i <= rect.y2)) {
                canvas[i][j] = '*';
            }
        }
    }
}

// Master rendering function
void renderAllShapes() {
    clearCanvas(); // Always clear the grid first
    
    // Loop through our database and draw every existing shape
    for (int i = 0; i < shapeCount; i++) {
        if (database[i].type == RECTANGLE) {
            drawRectangle(database[i]);
        }
        // Day 3 will handle Line, Circle, Triangle hooks here
    }
}

void displayCanvas() {
    #ifdef _WIN32
        system("cls");
    #else
        system("clear");
    #endif
    
    renderAllShapes(); // Render shapes before printing
    
    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COLS; j++) {
            printf("%c ", canvas[i][j]);
        }
        printf("\n");
    }
}

// --- OBJECT MANAGEMENT FUNCTIONS ---

void addObject() {
    if (shapeCount >= 100) {
        printf("Database full!\n");
        return;
    }
    
    Shape newShape;
    newShape.id = nextId++;
    
    printf("\nSelect Shape Type:\n1. Rectangle\n2. Line (Day 3)\n3. Circle (Day 3)\n4. Triangle (Day 3)\nChoice: ");
    int t;
    scanf("%d", &t);
    newShape.type = (ShapeType)t;
    
    if (newShape.type == RECTANGLE) {
        printf("Enter Top-Left Column (X1): "); scanf("%d", &newShape.x1);
        printf("Enter Top-Left Row (Y1): ");    scanf("%d", &newShape.y1);
        printf("Enter Bottom-Right Column (X2): "); scanf("%d", &newShape.x2);
        printf("Enter Bottom-Right Row (Y2): ");    scanf("%d", &newShape.y2);
        
        database[shapeCount] = newShape;
        shapeCount++;
        printf("Rectangle added successfully with ID: %d!\n", newShape.id);
    } else {
        printf("This shape logic is coming tomorrow on Day 3!\n");
    }
}

void deleteObject() {
    int targetId;
    printf("Enter the ID of the object to delete: ");
    scanf("%d", &targetId);
    
    int foundIndex = -1;
    for (int i = 0; i < shapeCount; i++) {
        if (database[i].id == targetId) {
            foundIndex = i;
            break;
        }
    }
    
    if (foundIndex == -1) {
        printf("Object ID not found!\n");
        return;
    }
    
    // Shift elements left to fill the gap in our array
    for (int i = foundIndex; i < shapeCount - 1; i++) {
        database[i] = database[i + 1];
    }
    shapeCount--;
    printf("Object %d deleted successfully!\n", targetId);
}

void modifyObject() {
    int targetId;
    printf("Enter the ID of the object to modify: ");
    scanf("%d", &targetId);
    
    for (int i = 0; i < shapeCount; i++) {
        if (database[i].id == targetId) {
            if (database[i].type == RECTANGLE) {
                printf("Modifying Rectangle ID %d\n", targetId);
                printf("Enter New Top-Left X1: "); scanf("%d", &database[i].x1);
                printf("Enter New Top-Left Y1: "); scanf("%d", &database[i].y1);
                printf("Enter New Bottom-Right X2: "); scanf("%d", &database[i].x2);
                printf("Enter New Bottom-Right Y2: "); scanf("%d", &database[i].y2);
                printf("Object modified successfully!\n");
                return;
            }
        }
    }
    printf("Object ID not found!\n");
}

void freeCanvas() {
    if (canvas != NULL) {
        for (int i = 0; i < ROWS; i++) {
            free(canvas[i]);
        }
        free(canvas);
    }
}

int main() {
    printf("Enter canvas rows: "); scanf("%d", &ROWS);
    printf("Enter canvas columns: "); scanf("%d", &COLS);

    initializeCanvas();
    clearCanvas();

    int choice = 0;
    while (choice != 5) {
        printf("\n--- 2D Graphics Editor (Day 2) ---\n");
        printf("1. Add Object\n");
        printf("2. Delete Object\n");
        printf("3. Modify Object\n");
        printf("4. Display Picture\n");
        printf("5. Exit\n");
        printf("Enter choice: ");
        scanf("%d", &choice);

        switch(choice) {
            case 1: addObject(); break;
            case 2: deleteObject(); break;
            case 3: modifyObject(); break;
            case 4: displayCanvas(); break;
            case 5: printf("Goodbye!\n"); break;
            default: printf("Invalid option.\n");
        }
    }

    freeCanvas();
    return 0;
}