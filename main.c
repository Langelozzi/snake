#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <termios.h>

// defining variables for the old terminal settings and new terminal settings/structure
struct termios oldTermSettings, newTermSettings;

struct snake {
    int x;
    int y;
};

struct fruit {
    int x;
    int y;
};

int height = 20, width = 20;
bool gameover;
int score;
struct snake snake;
struct fruit fruit;
int flag;

void placeFruit() {
    do {
        fruit.x = rand() % width;
    } while (fruit.x == 0);

    do {
        fruit.y = rand() % height;
    } while (fruit.y == 0);
}

void setUp() {
    gameover = false;
    score = 0;

    // store the position of snake (starts in middle)
    snake.x = height / 2;
    snake.y = width / 2;

    placeFruit();
}

void drawScreen() {
    system("clear");
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            if (
                i == 0 ||
                i == width - 1 ||
                j == 0 ||
                j == height -1
            ) {
                printf("#");
            }
            else {
                if (i == snake.y && j == snake.x) {
                    printf("0");
                } else if (i == fruit.y && j == fruit.x) {
                    printf("*");
                } else {
                    printf(" ");
                }
            }
        }
        printf("\n");
    }
    printf("Score: %d\n\n", score);
    printf("Press 'q' to quit");
}

void handleInput() {
    // getting the low level settings for stdin and placing in oldTermSettings
    tcgetattr(STDIN_FILENO, &oldTermSettings);
    newTermSettings = oldTermSettings;

    // setting the newTermSettings to be raw terminal (aka old fashion rules)
    // "input is available character by character, echoing is disabled, and no special processing"
    cfmakeraw(&newTermSettings);

    // setting the terminal IO settings to the newTermSettings
    tcsetattr(STDIN_FILENO, TCSANOW, &newTermSettings);

    int c = getchar();

    switch (c) {
        case 113: //q
            gameover = true;
            printf("bye\n");
            break;
        case 119: // w
            snake.y--;
            break;
        case 97: // a
            snake.x--;
            break;
        case 115: // s
            snake.y++;
            break;
        case 100: // d
            snake.x++;
            break;
        default:
            break;
    }

    // set the terminal structure/settings back to what it was
    tcsetattr(STDIN_FILENO, TCSANOW, &oldTermSettings);
}

void logic() {
    if (snake.x == fruit.x && snake.y == fruit.y) {
        score += 1;

        placeFruit();
    }

    // if they run into a wall
    if (
        snake.x == 0 ||
        snake.x == width - 1 ||
        snake.y == 0 ||
        snake.y == height - 1
    ) {
        gameover = true;
        printf("\n\n--------------Game Over!--------------");
    }
}

int main() {

    setUp();
    while (!gameover) {
        drawScreen();
        handleInput();
        logic();
    }

    // set the terminal structure/settings back to what it was after program finishes
    tcsetattr(STDIN_FILENO, TCSANOW, &oldTermSettings);
    return 0;
}
