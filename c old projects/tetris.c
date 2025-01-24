#include <stdio.h>
#include <stdlib.h>
#include <conio.h> // For Windows
#include <time.h>
#include <windows.h>

#define WIDTH 10
#define HEIGHT 20

char board[HEIGHT][WIDTH]; // Game board
int score = 0; // Score variable

void set_color(int color) {
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
}

typedef struct {
    int x, y;
} Point;

typedef struct {
    Point blocks[4];
    int color; // Color for the Tetromino
} Tetromino;

// Tetromino shapes
Tetromino shapes[] = {
    {{{0, 0}, {1, 0}, {2, 0}, {3, 0}}, 9}, // I 
    {{{0, 0}, {1, 0}, {0, 1}, {1, 1}}, 14}, // O 
    {{{0, 0}, {1, 0}, {2, 0}, {1, 1}}, 13}, // T  
    {{{0, 0}, {1, 0}, {1, 1}, {2, 1}}, 12}, // Z  
    {{{1, 0}, {2, 0}, {0, 1}, {1, 1}}, 10}  // S  
};

Tetromino current;
int currentX = 3, currentY = 0; // Initial position of the Tetromino

void initBoard() {
    for (int i = 0; i < HEIGHT; i++) {
        for (int j = 0; j < WIDTH; j++) {
            board[i][j] = ' ';
        }
    }
}

void drawBoard() {
    system("cls"); // Clear the terminal (Windows)

    // Display the score
    printf("Score: %d\n", score);

    for (int i = 0; i < HEIGHT; i++) {
        printf("|");
        for (int j = 0; j < WIDTH; j++) {
            if (board[i][j] == '#') {
                // Set color based on the block's position
                int color = 15; // Default (white) background color
                for (int k = 0; k < 4; k++) {
                    int x = current.blocks[k].x + currentX;
                    int y = current.blocks[k].y + currentY;
                    if (x == j && y == i) {
                        color = current.color; // Set color of the current block
                        break;
                    }
                }
                set_color(color);
            } else {
                set_color(15); // Set background color (white)
            }
            printf("%c", board[i][j]);
        }
        set_color(15); // Reset to default color
        printf("|\n");
    }
    for (int i = 0; i < WIDTH + 2; i++) printf("-");
    printf("\n");
}

void placeTetromino() {
    set_color(current.color); // Set color for the current Tetromino
    for (int i = 0; i < 4; i++) {
        int x = current.blocks[i].x + currentX;
        int y = current.blocks[i].y + currentY;
        if (y >= 0 && y < HEIGHT && x >= 0 && x < WIDTH) {
            board[y][x] = '#'; // Draw the block
        }
    }
    set_color(15); // Reset to default color (white) after placing the Tetromino
}

void clearTetromino() {
    for (int i = 0; i < 4; i++) {
        int x = current.blocks[i].x + currentX;
        int y = current.blocks[i].y + currentY;
        if (y >= 0 && y < HEIGHT && x >= 0 && x < WIDTH) {
            board[y][x] = ' ';
        }
    }
}

int canMove(int dx, int dy) {
    for (int i = 0; i < 4; i++) {
        int x = current.blocks[i].x + currentX + dx;
        int y = current.blocks[i].y + currentY + dy;
        if (x < 0 || x >= WIDTH || y >= HEIGHT || (y >= 0 && board[y][x] == '#')) {
            return 0;
        }
    }
    return 1;
}

void spawnTetromino() {
    current = shapes[rand() % 5];
    currentX = 3;
    currentY = 0;
    if (!canMove(0, 0)) {
        printf("Game Over!\n");
        exit(0);
    }
}

void lockTetromino() {
    set_color(current.color);
    for (int i = 0; i < 4; i++) {
        int x = current.blocks[i].x + currentX;
        int y = current.blocks[i].y + currentY;
        if (y >= 0 && y < HEIGHT && x >= 0 && x < WIDTH) {
            board[y][x] = '#';
        }
    }
    set_color(15); // Reset to default color
}

void clearLines() {
    for (int i = 0; i < HEIGHT; i++) {
        int full = 1;
        for (int j = 0; j < WIDTH; j++) {
            if (board[i][j] == ' ') {
                full = 0;
                break;
            }
        }
        if (full) {
            for (int k = i; k > 0; k--) {
                for (int j = 0; j < WIDTH; j++) {
                    board[k][j] = board[k - 1][j];
                }
            }
            for (int j = 0; j < WIDTH; j++) {
                board[0][j] = ' ';
            }
            // Increase the score for each cleared line
            score += 100; // Add 100 points, you can change this value if needed
        }
    }
}

void rotateTetromino() {
    for (int i = 0; i < 4; i++) {
        int x = current.blocks[i].x;
        int y = current.blocks[i].y;
        // Rotate 90 degrees
        current.blocks[i].x = -y;
        current.blocks[i].y = x;
    }
    // If the rotation makes movement impossible, revert to the previous state
    if (!canMove(0, 0)) {
        for (int i = 0; i < 4; i++) {
            int x = current.blocks[i].x;
            int y = current.blocks[i].y;
            current.blocks[i].x = y;
            current.blocks[i].y = -x;
        }
    }
}

void gameLoop() {
    spawnTetromino();
    while (1) {
        clearTetromino();
        if (_kbhit()) {
            char key = _getch();
            if (key == 'a' && canMove(-1, 0)) currentX--; // Move left
            if (key == 'd' && canMove(1, 0)) currentX++;  // Move right
            if (key == 's' && canMove(0, 1)) currentY++;  // Move down
            if (key == 'w') rotateTetromino();            // Rotate
        }
        if (canMove(0, 1)) {
            currentY++; // Move down
        } else {
            lockTetromino();
            clearLines();
            spawnTetromino();
        }
        placeTetromino(); // Place the Tetromino
        drawBoard();      // Update the board and show it
        Sleep(200);       // Slow down the game loop to control speed
    }
}

int main() {
    srand(time(0));
    initBoard();
    gameLoop();
    return 0;
}

