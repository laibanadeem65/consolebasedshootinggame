#include <iostream>
#include <conio.h>
#include <windows.h>
#include <ctime>

using namespace std;

const int width = 30;
const int height = 10;
const int numEnemies = 5;

bool gameOver;
int playerY;
int bulletX, bulletY;
bool bulletActive;
int score;

int enemyX[numEnemies];
int enemyY[numEnemies];
bool enemyAlive[numEnemies];

int enemyTimer = 0;

void SetColor(int color) {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, color);
}

void Setup() {
    gameOver = false;
    playerY = height / 2;
    bulletActive = false;
    score = 0;
    enemyTimer = 0;

    for (int i = 0; i < numEnemies; i++) {
        enemyX[i] = width - 2 - (i * 5);
        enemyY[i] = rand() % height;
        enemyAlive[i] = true;
    }
}

void Draw() {
    system("cls");

    SetColor(9);
    for (int i = 0; i < width + 2; i++) cout << "#";
    cout << endl;

    for (int i = 0; i < height; i++) {
        cout << "#";
        for (int j = 0; j < width; j++) {
            bool drawn = false;

          
            if (i == playerY && j == 0) {
                SetColor(10); cout << ">"; drawn = true;
            }

           
            if (!drawn && bulletActive && i == bulletY && j == bulletX) {
                SetColor(14); cout << "-"; drawn = true;
            }

          
            for (int k = 0; k < numEnemies; k++) {
                if (enemyAlive[k] && i == enemyY[k] && j == enemyX[k]) {
                    SetColor(12); cout << "<"; drawn = true;
                    break;
                }
            }

          
            if (!drawn && rand() % 50 == 0) {
                SetColor(8); cout << "."; drawn = true;
            }

            if (!drawn) cout << " ";
        }
        cout << "#\n";
    }

    SetColor(9);
    for (int i = 0; i < width + 2; i++) cout << "#";
    cout << endl;

    SetColor(7);
    cout << "Score: " << score << endl;
    cout << "Controls: W-Up S-Down D-Fire X-Quit" << endl;
}

void Input() {
    if (_kbhit()) {
        char ch = _getch();
        switch (tolower(ch)) {
            case 'w': if (playerY > 0) playerY--; break;
            case 's': if (playerY < height - 1) playerY++; break;
            case 'd':
                if (!bulletActive) {
                    bulletActive = true;
                    bulletY = playerY;
                    bulletX = 1;
                }
                break;
            case 'x': gameOver = true; break;
        }
    }
}

void Logic() {
    if (bulletActive) {
        bulletX++;
        if (bulletX >= width)
            bulletActive = false;
    }

    enemyTimer++;
    if (enemyTimer >= 5) {
        for (int i = 0; i < numEnemies; i++) {
            if (enemyAlive[i]) {
                enemyX[i]--;
                if (enemyX[i] <= 0 && enemyY[i] == playerY) {
                    gameOver = true;
                }
                if (enemyX[i] < 0) {
                    enemyX[i] = width - 1;
                    enemyY[i] = rand() % height;
                }
            }
        }
        enemyTimer = 0;
    }

    for (int i = 0; i < numEnemies; i++) {
        if (enemyAlive[i] && bulletActive &&
            bulletX == enemyX[i] && bulletY == enemyY[i]) {
            enemyAlive[i] = false;
            bulletActive = false;
            score += 10;
        }
    }
}

bool AllEnemiesDead() {
    for (int i = 0; i < numEnemies; i++)
        if (enemyAlive[i])
            return false;
    return true;
}

int main() {
    srand(time(0));
    Setup();

    while (!gameOver) {
        Draw();
        Input();
        Logic();
        if (AllEnemiesDead()) break;
        Sleep(50);
    }

    system("cls");
    SetColor(12);
    if (AllEnemiesDead())
        cout << "?? Victory! All enemies down! Score: " << score << endl;
    else
        cout << "?? You were hit! Final Score: " << score << endl;

    SetColor(7);
    return 0;
}

