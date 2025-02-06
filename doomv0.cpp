#include <iostream>
#include <conio.h>
#include <windows.h>
#include <vector>
#include <cstdlib>
#include <ctime>
using namespace std;
const int WIDTH = 20 ;
const int HEIGHT = 10;

const char PLAYER = 'A';
const char ENEMY = 'E';
const char BULLET = '|';
const char WALL = '#';
const char EMPTY = ' ';

struct Position {
	int x, y;
};

class Player {
public:
	Position pos;
	int health;
	int score;
	Player() : health(100), score(0) {
		pos.x = WIDTH / 2;
		pos.y = HEIGHT - 1;
	}
	void moveLeft() {
		if (pos.x > 1) pos.x--;
	}
	void moveRight() {
		if (pos.x < WIDTH - 2) pos.x++;
	}
	void shoot(vector<Position>& bullets) {
		bullets.push_back({ pos.x, pos.y - 1 });
	}
};

class Enemy {
public:
	Position pos;
	bool active;
	Enemy(int x, int y) : active(true) {
		pos.x = x;
		pos.y = y;
	}
	void move() {
		pos.y++;
	}
};

void drawMap(const Player& player, const vector<Enemy>& enemies, const vector<Position>& bullets) {
	system("cls");
	for (int x = 0; x < WIDTH; x++)  std::cout << WALL;
	std::cout << endl;

	for (int y = 0; y < HEIGHT; y++) {
		std::cout << WALL;
		for (int x = 0; x < WIDTH - 2; x++) {
			if (x == player.pos.x && y == player.pos.y) {
				std::cout << PLAYER;
				continue;
			}
			bool isBullet = false;
			for (const auto& bullet : bullets) {
				if (x == bullet.x && y == bullet.y) {
					std::cout << BULLET;
					isBullet = true;
					break;
				}
			}
			if (isBullet) continue;
			bool isEnemy = false;
			for (const auto& enemy : enemies) {
				if (enemy.active && x == enemy.pos.x && y == enemy.pos.y) {
					std::cout << ENEMY;
					isEnemy = true;
					break;
				}
			}
			if (isEnemy) continue;
			std::cout << EMPTY;
		}
		std::cout << WALL;
		std::cout << endl;
	}
	for (int x = 0; x < WIDTH; x++) std::cout << WALL;
	std::cout << endl;
	std::cout << "Health: " << player.health << " | Score: " << player.score << endl;
}

int main() {
	srand(static_cast<unsigned int>(time(0)));
	Player player;
	vector<Enemy> enemies;
	vector<Position> bullets;
	int enemySpawnDelay = 0;
	int gameSpeed = 50;
	while (player.health > 0) {
		if (_kbhit()) {
			char key = _getch();
			switch (key) {
				case 'a': player.moveLeft(); break;
				case 'd': player.moveRight(); break;
				case ' ': player.shoot(bullets); break;
				case 'q': return 0;
			}
		}
		if (enemySpawnDelay <= 0) {
			enemies.push_back(Enemy(1 + rand() % (WIDTH- 2), 0));
			enemySpawnDelay = 10 + rand() % 10;
		}
		enemySpawnDelay--;
		for (auto& enemy : enemies) {
			enemy.move();
			if (enemy.pos.y >= HEIGHT-1) {
				player.health -= 5;
				enemy.active = false;
			}
		}
		for (auto& bullet : bullets) {
			bullet.y--;
			for (auto& enemy : enemies) {
				if (enemy.active && bullet.x == enemy.pos.x && bullet.y == enemy.pos.y) {
					enemy.active = false;
					player.score += 10;
					bullet.y = -1;
				}
			}
		}
		enemies.erase(remove_if(enemies.begin(), enemies.end(), [](const Enemy& e) { return !e.active; }), enemies.end());
		bullets.erase(remove_if(bullets.begin(), bullets.end(), [](const Position& b) { return b.y < 0; }), bullets.end());
		drawMap(player, enemies, bullets);
		Sleep(gameSpeed);
	}
	cout << "Game Over! Final Score: " << player.score << endl;
	return 0;
}

