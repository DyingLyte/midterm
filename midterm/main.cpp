#include <iostream>
#include <conio.h>
#include <windows.h>
#include <vector>
#include <ctime>

using namespace std;

// Game Configuration
bool gameOver;
const int width = 20;
const int height = 17;

// Snake & Fruit Positions
int headX, headY, fruitX, fruitY, score;
int tailX[100], tailY[100];
int nTail;

enum eDirection { STOP = 0, LEFT, RIGHT, UP, DOWN };
eDirection dir;

void Setup() {
    srand(time(NULL));
    gameOver = false;
    dir = STOP;
    headX = width / 2;
    headY = height / 2;

    // Spawn initial fruit randomly
    fruitX = rand() % width;
    fruitY = rand() % height;
    score = 0;
    nTail = 0;
}

void Draw() {
    // Move cursor to the top-left instead of using system("cls") to prevent flickering
    COORD cursorPosition;
    cursorPosition.X = 0;
    cursorPosition.Y = 0;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), cursorPosition);

    // Top wall
    for (int i = 0; i < width + 2; i++) cout << "#";
    cout << endl;

    // Map body
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            // Left wall
            if (j == 0) cout << "#";

            // Snake Head
            if (i == headY && j == headX) {
                cout << "O";
            }
            // Fruit
            else if (i == fruitY && j == fruitX) {
                cout << "X";
            }
            // Snake Tail
            else {
                bool printTail = false;
                for (int k = 0; k < nTail; k++) {
                    if (tailX[k] == j && tailY[k] == i) {
                        cout << "o";
                        printTail = true;
                        break;
                    }
                }
                if (!printTail) cout << " ";
            }

            // Right wall
            if (j == width - 1) cout << "#";
        }
        cout << endl;
    }

    // Bottom wall
    for (int i = 0; i < width + 2; i++) cout << "#";
    cout << endl;

    // Display Score
    cout << "Score: " << score << "     " << endl;
}

void Input() {
    // Check if a keyboard key has been pressed
    if (_kbhit()) {
        switch (_getch()) {
        case 'a': if (dir != RIGHT) dir = LEFT; break;
        case 'd': if (dir != LEFT) dir = RIGHT; break;
        case 'w': if (dir != DOWN) dir = UP; break;
        case 's': if (dir != UP) dir = DOWN; break;
        case 'x': gameOver = true; break; // Quit game
        }
    }
}

void Logic() {
    int prevX = tailX[0];
    int prevY = tailY[0];
    int prev2X, prev2Y;
    tailX[0] = headX;
    tailY[0] = headY;

    // Move tail array forward
    for (int i = 1; i < nTail; i++) {
        prev2X = tailX[i];
        prev2Y = tailY[i];
        tailX[i] = prevX;
        tailY[i] = prevY;
        prevX = prev2X;
        prevY = prev2Y;
    }

    // Move Snake Head
    switch (dir) {
    case LEFT:  headX--; break;
    case RIGHT: headX++; break;
    case UP:    headY--; break;
    case DOWN:  headY++; break;
    default: break;
    }

    // Collision Detection: Walls
    if (headX >= width || headX < 0 || headY >= height || headY < 0) {
        gameOver = true;
    }

    // Collision Detection: Self (Tail)
    for (int i = 0; i < nTail; i++) {
        if (tailX[i] == headX && tailY[i] == headY) {
            gameOver = true;
        }
    }

    // Eating Fruit Logic
    if (headX == fruitX && headY == fruitY) {
        score += 10;
        nTail++;
        fruitX = rand() % width;
        fruitY = rand() % height;
    }
}

int main() {
    Setup();

    // Hide console cursor for cleaner rendering
    HANDLE out = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO cursorInfo;
    GetConsoleCursorInfo(out, &cursorInfo);
    cursorInfo.bVisible = false;
    SetConsoleCursorInfo(out, &cursorInfo);

    // Main Game Loop
    while (!gameOver) {
        Draw();
        Input();
        Logic();
        Sleep(100); // Controls game speed (lower number = faster game)
    }

    // Game Over screen
    system("cls");
    cout << "====================" << endl;
    cout << "     GAME OVER      " << endl;
    cout << "====================" << endl;
    cout << "Final Score: " << score << endl;

    return 0;
}
