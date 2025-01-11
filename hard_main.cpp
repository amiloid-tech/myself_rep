#include <iostream>
#include <vector>
#include <cstdlib>
#include <chrono>
#include <thread>

#ifdef _WIN32
#include <windows.h>
#else
#include <cstdlib>
#endif

using namespace std;

const int ROWS = 20;
const int COLS = 100;

void clearScreen() {
#ifdef _WIN32
    system("cls"); 
#else
    system("clear"); 
#endif
}

void initialize(vector<vector<int>>& board, bool randomFill = true) {
    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COLS; j++) {
            if (randomFill) {
                board[i][j] = rand() % 2; 
            }
            else {
                board[i][j] = (i == ROWS / 2 && j > COLS / 4 && j < 3 * COLS / 4) ? 1 : 0; 
            }
        }
    }
}

void printBoard(const vector<vector<int>>& board) {
    clearScreen(); 
    for (const auto& row : board) {
        for (const auto& cell : row) {
            cout << (cell ? 'O' : '.');
        }
        cout << endl;
    }
}

int countNeighbors(const vector<vector<int>>& board, int x, int y) {
    int count = 0;
    for (int i = -1; i <= 1; i++) {
        for (int j = -1; j <= 1; j++) {
            if (i == 0 && j == 0) continue; 
            int newX = x + i;
            int newY = y + j;
            
            if (newX >= 0 && newX < ROWS && newY >= 0 && newY < COLS) {
                count += board[newX][newY];
            }
        }
    }
    return count;
}

void updateBoard(vector<vector<int>>& board) {
    vector<vector<int>> newBoard(ROWS, vector<int>(COLS, 0));

    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COLS; j++) {
            int neighbors = countNeighbors(board, i, j);
            if (board[i][j] == 1) { 
                newBoard[i][j] = (neighbors == 2 || neighbors == 3) ? 1 : 0;
            }
            else { 
                newBoard[i][j] = (neighbors == 3) ? 1 : 0;
            }
        }
    }

    
    board = newBoard;
}

void gameLoop(vector<vector<int>>& board) {
    while (true) {
        printBoard(board);
        updateBoard(board);
        this_thread::sleep_for(chrono::milliseconds(100)); 
    }
}

int main() {
    vector<vector<int>> board(ROWS, vector<int>(COLS));
    srand(static_cast<unsigned int>(time(0)));

    char choice;
    cout << "(r - random, s - shar): ";
    cin >> choice;

    if (choice == 's') {
        initialize(board, false); 
    }
    else {
        initialize(board, true); 
    }

    gameLoop(board);

    return 0;
}
