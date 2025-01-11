
#include <iostream>
#include <conio.h>
#include <cstdlib>
#include <ctime>
#include <vector>
#include <windows.h> 

class SnakeGame {
private:
    int width, height;
    int snakeLength;
    std::vector<int> snakeX, snakeY;
    int foodX, foodY;
    int direction; // 0: вверх, 1: вправо, 2: вниз, 3: влево
    std::vector<std::pair<int, int>> obstacles;

    void generateFood() {
        do {
            foodX = rand() % width;
            foodY = rand() % height;
        } while (isOccupied(foodX, foodY));
    }

    bool isOccupied(int x, int y) {
        for (int i = 0; i < snakeLength; ++i) {
            if (snakeX[i] == x && snakeY[i] == y) return true;
        }
        for (const auto& obs : obstacles) {
            if (obs.first == x && obs.second == y) return true;
        }
        return false;
    }

    void createObstacles(int num) {
        while (obstacles.size() < num) {
            int obsX = rand() % width;
            int obsY = rand() % height;
            if (!isOccupied(obsX, obsY)) {
                obstacles.push_back({ obsX, obsY });
            }
        }
    }

    void moveSnake() {
        for (int i = snakeLength; i > 0; --i) {
            snakeX[i] = snakeX[i - 1];
            snakeY[i] = snakeY[i - 1];
        }
        if (direction == 0) snakeY[0]--;
        if (direction == 1) snakeX[0]++;
        if (direction == 2) snakeY[0]++;
        if (direction == 3) snakeX[0]--;
    }

    bool checkCollision() {
        if (snakeX[0] < 0 || snakeX[0] >= width || snakeY[0] < 0 || snakeY[0] >= height)
            return true;
        for (int i = 1; i < snakeLength; ++i) {
            if (snakeX[0] == snakeX[i] && snakeY[0] == snakeY[i])
                return true;
        }
        for (const auto& obs : obstacles) {
            if (snakeX[0] == obs.first && snakeY[0] == obs.second)
                return true;
        }
        return false;
    }

public:
    SnakeGame(int w, int h, int level) : width(w), height(h), snakeLength(1), direction(1) {
        snakeX.resize(w * h);
        snakeY.resize(w * h);
        snakeX[0] = w / 2;
        snakeY[0] = h / 2;
        generateFood();
        createObstacles(level);
        srand(static_cast<unsigned>(time(0)));
    }

    void changeDirection(int dir) {
        if (dir == 0 && direction != 2) direction = dir;
        else if (dir == 1 && direction != 3) direction = dir;
        else if (dir == 2 && direction != 0) direction = dir;
        else if (dir == 3 && direction != 1) direction = dir;
    }

    void display() {
        system("cls"); 
        for (int y = 0; y < height; y++) {
            for (int x = 0; x < width; x++) {
                bool printed = false;
                if (x == foodX && y == foodY) {
                    std::cout << "F";
                    printed = true;
                }
                for (int i = 0; i < snakeLength; i++) {
                    if (snakeX[i] == x && snakeY[i] == y) {
                        std::cout << "S";
                        printed = true;
                    }
                }
                for (const auto& obs : obstacles) {
                    if (obs.first == x && obs.second == y) {
                        std::cout << "X";
                        printed = true;
                    }
                }
                if (!printed) std::cout << ".";
            }
            std::cout << "\n";
        }
        std::cout << "Используйте W, A, S, D для управления...\n";
    }

    void run() {
        while (true) {
            moveSnake();

            if (checkCollision()) {
                std::cout << "Игра окончена! Вы набрали " << snakeLength - 1 << " очков.\n";
                break;
            }

            if (snakeX[0] == foodX && snakeY[0] == foodY) {
                snakeLength++;
                generateFood();
            }

            display();

           
            if (_kbhit()) {
                char c = _getch();
                if (c == 'w') changeDirection(0);
                else if (c == 'd') changeDirection(1);
                else if (c == 's') changeDirection(2);
                else if (c == 'a') changeDirection(3);
            }

            Sleep(50); 
        }
    }
};

int main() {
    int level;
    std::cout << "Введите уровень сложности (количество препятствий): ";
    std::cin >> level;

    SnakeGame game(50, 40, level);
    game.run();
    return 0;
}

