#include <stdio.h>
#include <stdlib.h>
#include <math.h> // Needed for roundf() and sqrt() in circle/line math

// Global Canvas variables
int ROWS;
int COLS;
char** canvas = NULL;

typedef enum { RECTANGLE = 1, LINE, CIRCLE, TRIANGLE } ShapeType;

typedef struct {
    int id;            
    ShapeType type;    
    int x1, y1;        // Line start / Rect top-left / Circle center / Triangle point A
    int x2, y2;        // Line end / Rect bottom-right / Circle radius (x2=radius) / Triangle point B
    int x3, y3;        // Triangle point C
} Shape;

Shape database[100];   
int shapeCount = 0;    
int nextId = 1;        

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

void clearInputBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

// Safe plot function to ensure we don't write outside the allocated array boundaries
void plot(int x, int y) {
    if (x >= 0 && x < COLS && y >= 0 && y < ROWS) {
        canvas[y][x] = '*';
    }
}

// --- DAY 3: DRAWING ALGORITHMS ---

// 1. Line Drawing (Digital Differential Analyzer approach)
void drawLine(int x1, int y1, int x2, int y2) {
    int dx = x2 - x1;
    int dy = y2 - y1;
    int steps = abs(dx) > abs(dy) ? abs(dx) : abs(dy);

    if (steps == 0) {
        plot(x1, y1);
        return;
    }

    float xInc = dx / (float)steps;
    float yInc = dy / (float)steps;

    float x = x1;
    float y = y1;
    for (int i = 0; i <= steps; i++) {
        plot((int)roundf(x), (int)roundf(y));
        x += xInc;
        y += yInc;
    }
}

// 2. Rectangle Drawing
void drawRectangle(Shape rect) {
    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COLS; j++) {
            if ((i == rect.y1 || i == rect.y2) && (j >= rect.x1 && j <= rect.x2)) {
                plot(j, i);
            }
            if ((j == rect.x1 || j == rect.x2) && (i >= rect.y1 && i <= rect.y2)) {
                plot(j, i);
            }
        }
    }
}

// 3. Circle Drawing (Midpoint / Midpoint Circle algorithm logic)
void drawCircle(Shape circ) {
    int cx = circ.x1;
    int cy = circ.y1;
    int r = circ.x2; // We store radius in x2

    int x = 0;
    int y = r;
    int d = 3 - 2 * r;

    while (y >= x) {
        // Plot all 8 symmetric points of the circle perimeter
        plot(cx + x, cy + y); plot(cx - x, cy + y);
        plot(cx + x, cy - y); plot(cx - x, cy - y);
        plot(cx + y, cy + x); plot(cx - y, cy + x);
        plot(cx + y, cy - x); plot(cx - y, cy - x);
        x++;

        if (d > 0) {
            y--;
            d = d + 4 * (x - y) + 10;
        } else {
            d = d + 4 * x + 6;
        }
    }
}

// 4. Triangle Drawing (Connects 3 lines)
void drawTriangle(Shape tri) {
    drawLine(tri.x1, tri.y1, tri.x2, tri.y2); // Line A to B
    drawLine(tri.x2, tri.y2, tri.x3, tri.y3); // Line B to C
    drawLine(tri.x3, tri.y3, tri.x1, tri.y1); // Line C to A
}

// Master rendering loop
void renderAllShapes() {
    clearCanvas(); 
    for (int i = 0; i < shapeCount; i++) {
        switch (database[i].type) {
            case RECTANGLE: drawRectangle(database[i]); break;
            case LINE:      drawLine(database[i].x1, database[i].y1, database[i].x2, database[i].y2); break;
            case CIRCLE:    drawCircle(database[i]); break;
            case TRIANGLE:  drawTriangle(database[i]); break;
        }
    }
}

void displayCanvas() {
    #ifdef _WIN32
        system("cls");
    #else
        system("clear");
    #endif
    
    renderAllShapes(); 
    
    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COLS; j++) {
            printf("%c ", canvas[i][j]);
        }
        printf("\n");
    }
}

// --- MANAGEMENT FUNCTIONS ---

void addObject() {
    if (shapeCount >= 100) {
        printf("Database full!\n");
        return;
    }
    
    Shape newShape;
    newShape.id = nextId++;
    
    printf("\nSelect Shape Type:\n1. Rectangle\n2. Line\n3. Circle\n4. Triangle\nChoice: ");
    int t;
    scanf("%d", &t);
    clearInputBuffer(); // Clear buffer after reading the shape type choice
    newShape.type = (ShapeType)t;
    
    switch (newShape.type) {
        case RECTANGLE:
            printf("Enter Top-Left Column (X1): "); scanf("%d", &newShape.x1);
            printf("Enter Top-Left Row (Y1): ");    scanf("%d", &newShape.y1);
            printf("Enter Bottom-Right Column (X2): "); scanf("%d", &newShape.x2);
            printf("Enter Bottom-Right Row (Y2): ");    scanf("%d", &newShape.y2);
            clearInputBuffer(); // Clear buffer after coordinates
            break;
        case LINE:
            printf("Enter Start Column (X1): "); scanf("%d", &newShape.x1);
            printf("Enter Start Row (Y1): ");    scanf("%d", &newShape.y1);
            printf("Enter End Column (X2): ");   scanf("%d", &newShape.x2);
            printf("Enter End Row (Y2): ");      scanf("%d", &newShape.y2);
            clearInputBuffer();
            break;
        case CIRCLE:
            printf("Enter Center Column (X1): "); scanf("%d", &newShape.x1);
            printf("Enter Center Row (Y1): ");    scanf("%d", &newShape.y1);
            printf("Enter Radius: ");             scanf("%d", &newShape.x2); 
            clearInputBuffer();
            break;
        case TRIANGLE:
            printf("Enter Pt 1 Column (X1) & Row (Y1): "); scanf("%d %d", &newShape.x1, &newShape.y1);
            printf("Enter Pt 2 Column (X2) & Row (Y2): "); scanf("%d %d", &newShape.x2, &newShape.y2);
            printf("Enter Pt 3 Column (X3) & Row (Y3): "); scanf("%d %d", &newShape.x3, &newShape.y3);
            clearInputBuffer();
            break;
        default:
            printf("Invalid shape type selected.\n");
            return;
    }
    
    database[shapeCount++] = newShape;
    printf("Object added successfully with assigned ID: %d!\n", newShape.id);
}

void deleteObject() {
    int targetId;
    printf("Enter Object ID to delete: ");
    scanf("%d", &targetId);
    
    int index = -1;
    for (int i = 0; i < shapeCount; i++) {
        if (database[i].id == targetId) {
            index = i;
            break;
        }
    }
    if (index == -1) { printf("ID not found!\n"); return; }
    
    for (int i = index; i < shapeCount - 1; i++) {
        database[i] = database[i + 1];
    }
    shapeCount--;
    printf("Object %d deleted.\n", targetId);
}

void modifyObject() {
    int targetId;
    printf("Enter Object ID to modify: ");
    scanf("%d", &targetId);
    
    for (int i = 0; i < shapeCount; i++) {
        if (database[i].id == targetId) {
            printf("Modifying object ID %d. Re-enter parameters:\n", targetId);
            if (database[i].type == RECTANGLE || database[i].type == LINE) {
                printf("Enter X1, Y1, X2, Y2: ");
                scanf("%d %d %d %d", &database[i].x1, &database[i].y1, &database[i].x2, &database[i].y2);
            } else if (database[i].type == CIRCLE) {
                printf("Enter Center X1, Center Y1, Radius: ");
                scanf("%d %d %d", &database[i].x1, &database[i].y1, &database[i].x2);
            } else if (database[i].type == TRIANGLE) {
                printf("Enter Pt1 (X1 Y1), Pt2 (X2 Y2), Pt3 (X3 Y3): ");
                scanf("%d %d %d %d %d %d", &database[i].x1, &database[i].y1, &database[i].x2, &database[i].y2, &database[i].x3, &database[i].y3);
            }
            printf("Object updated successfully!\n");
            return;
        }
    }
    printf("Object ID not found!\n");
}

void freeCanvas() {
    if (canvas != NULL) {
        for (int i = 0; i < ROWS; i++) { free(canvas[i]); }
        free(canvas);
    }
}

int main() {
    printf("Enter canvas rows: ");  scanf("%d", &ROWS);
    printf("Enter canvas columns: "); scanf("%d", &COLS);
    clearInputBuffer(); // Clean up initial inputs

    initializeCanvas();

    int choice = 0;
    while (choice != 5) {
        printf("\n--- 2D Graphics Editor (Complete) ---\n");
        printf("1. Add Object\n");
        printf("2. Delete Object\n");
        printf("3. Modify Object\n");
        printf("4. Display Picture\n");
        printf("5. Exit\n");
        printf("Enter choice: ");
        
        if (scanf("%d", &choice) != 1) {
            // If the user inputs a character instead of a number, don't infinite loop
            clearInputBuffer();
            printf("Invalid selection. Please enter a number.\n");
            continue;
        }
        clearInputBuffer(); // CRITICAL: This wipes the buffer right after choosing a menu option!

        switch(choice) {
            case 1: addObject(); break;
            case 2: deleteObject(); break;
            case 3: modifyObject(); break;
            case 4: displayCanvas(); break;
            case 5: printf("Exiting software.\n"); break;
            default: printf("Invalid selection.\n");
        }
    }

    freeCanvas();
    return 0;
}