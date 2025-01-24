#include <stdio.h>
#include <conio.h>
#include <windows.h>
#include <stdlib.h>
#include <time.h>

#define BOARD_WIDTH 30
#define BOARD_HEIGHT 15
#define INITIAL_SNAKE_LENGTH 5

void gotoxy(int x, int y) {
    COORD coord;
    coord.X = x;
    coord.Y = y;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

void set_color(int color) {
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
}

int check_collision(int x, int y, int snake_x[], int snake_y[], int length) {
    // Check wall collision
    if (x < 0 || x >= BOARD_WIDTH || y < 0 || y >= BOARD_HEIGHT) {
        return 1;
    }
    // Check self-collision
    for (int i = 0; i < length; i++) {
        if (snake_x[i] == x && snake_y[i] == y) {
            return 1;
        }
    }
    return 0;
}

void generate_food(int *food_x, int *food_y, int snake_x[], int snake_y[], int length) {
    int valid_position = 0;
    while (!valid_position) {
        *food_x = rand() % BOARD_WIDTH;
        *food_y = rand() % BOARD_HEIGHT;
        valid_position = 1;

        // Ensure food does not spawn on the snake
        for (int i = 0; i < length; i++) {
            if (snake_x[i] == *food_x && snake_y[i] == *food_y) {
                valid_position = 0;
                break;
            }
        }
    }
}

int main() {
    int snake_x[BOARD_WIDTH * BOARD_HEIGHT] = {15, 14, 13, 12, 11};
    int snake_y[BOARD_WIDTH * BOARD_HEIGHT] = {8, 8, 8, 8, 8};
    int direction_x = 1, direction_y = 0;
    char input, symbol = '*';
    int game_over = 0;
    int snake_length = INITIAL_SNAKE_LENGTH;

    int food_x, food_y;
    srand(time(NULL));
    generate_food(&food_x, &food_y, snake_x, snake_y, snake_length);

    // Draw initial snake and food
    set_color(10); // Set snake color to green
    for (int i = 0; i < snake_length; i++) {
        gotoxy(snake_x[i], snake_y[i]);
        printf("%c", symbol);
    }
    set_color(12); // Set food color to red
    gotoxy(food_x, food_y);
    printf("O");

    while (!game_over) {
        // Get user input
        if (_kbhit()) {
            input = _getch();
            switch (input) {
                case '2': // Down
                    if (direction_y != -1) {
                        direction_x = 0;
                        direction_y = 1;
                    }
                    break;
                case '8': // Up
                    if (direction_y != 1) {
                        direction_x = 0;
                        direction_y = -1;
                    }
                    break;
                case '4': // Left
                    if (direction_x != 1) {
                        direction_x = -1;
                        direction_y = 0;
                    }
                    break;
                case '6': // Right
                    if (direction_x != -1) {
                        direction_x = 1;
                        direction_y = 0;
                    }
                    break;
            }
        }

        // Calculate new head position
        int new_head_x = snake_x[0] + direction_x;
        int new_head_y = snake_y[0] + direction_y;

        // Check for collisions
        if (check_collision(new_head_x, new_head_y, snake_x, snake_y, snake_length)) {
            game_over = 1;
            break;
        }

        // Check if snake eats food
        if (new_head_x == food_x && new_head_y == food_y) {
            snake_length++;
            generate_food(&food_x, &food_y, snake_x, snake_y, snake_length);
            set_color(12); // Set food color to red
            gotoxy(food_x, food_y);
            printf("O");
        } else {
            // Clear the tail
            gotoxy(snake_x[snake_length - 1], snake_y[snake_length - 1]);
            printf(" ");
        }

        // Draw new head
        set_color(10); // Set snake color to green
        gotoxy(new_head_x, new_head_y);
        printf("%c", symbol);

        // Update snake position
        for (int i = snake_length - 1; i > 0; i--) {
            snake_x[i] = snake_x[i - 1];
            snake_y[i] = snake_y[i - 1];
        }
        snake_x[0] = new_head_x;
        snake_y[0] = new_head_y;

        // Delay
        Sleep(150);
    }

    // Game over
    gotoxy(0, BOARD_HEIGHT + 1);
    printf("Game Over! The snake collided with itself or the walls.\n");
    return 0;
}

