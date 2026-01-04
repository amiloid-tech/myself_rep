#define _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_DEPRECATE
#pragma warning(disable : 4996)

#include <iostream>
#include <sstream>
#include <string>
#include <limits>
#include <vector>
#include <cctype>
#include <iomanip>
#include <windows.h>
#include <conio.h>
#include <algorithm>
#include <ctime>
#include <map>
#include <cmath>
#include <stack>
#include <fstream>
#include <psapi.h>
#include <chrono>
#include <direct.h>
#include <random>
#include <tlhelp32.h>
#include <mmsystem.h>
#include <process.h>
#include <pdh.h>
#pragma comment(lib, "winmm.lib")
#pragma comment(lib, "psapi.lib")
#pragma comment(lib, "pdh.lib")
using namespace std;

enum ConsoleColor {
    BLACK = 0, BLUE = 1, GREEN = 2, CYAN = 3, RED = 4,
    MAGENTA = 5, BROWN = 6, LIGHT_GRAY = 7, DARK_GRAY = 8,
    LIGHT_BLUE = 9, LIGHT_GREEN = 10, LIGHT_CYAN = 11,
    LIGHT_RED = 12, LIGHT_MAGENTA = 13, YELLOW = 14, WHITE = 15,
    LIGHT_YELLOW = 14
};

struct Calculation {
    string expression;
    double result;
    string time;
    string hex_result;
    string bin_result;
};

struct SystemInfo {
    double cpuLoad;
    int memoryUsage;
    long long freeMemory;
    long long totalMemory;
    int processCount;
    string uptime;
    double appMemory;
};

struct App {
    int id;
    string name;
    string description;
};

struct FileInfo {
    string name;
    string type;
    long long size;
    string modified;
    bool isDirectory;
    bool isTextFile;
    bool isDrawingFile;
    bool isMusicFile;
};

struct Task {
    int id;
    string name;
    string description;
    bool completed;
};

struct MemoryCard {
    int value;
    bool revealed;
    bool matched;
    int x, y;
};

struct ProcessInfo {
    DWORD pid;
    string name;
    DWORD memoryUsage;
};

struct PaintPoint {
    char ch;
    ConsoleColor color;
    bool active;
};

struct Song {
    string title;
    string artist;
    int duration;
    vector<int> notes;
};

struct MusicNote {
    int pitch;
    int duration;
    int delay;
};

struct MusicComposition {
    string name;
    vector<MusicNote> notes;
    bool isPlaying;
    int currentNote;
};

struct WeatherData {
    string city;
    double temperature;
    double humidity;
    string condition;
    double windSpeed;
    string forecast;
};

struct SnakeSegment {
    int x, y;
};

struct CalendarEvent {
    int day;
    int month;
    string description;
};

struct ClockData {
    bool showAnalog;
    bool alarmSet;
    int alarmHour;
    int alarmMinute;
    int timerSeconds;
    bool timerRunning;
    time_t timerStart;
};

struct QuestData {
    int currentStep;
    bool hasKey;
    bool hasGift;
    bool metSanta;
    bool puzzleSolved;
};

struct SettingsData {
    string fontName;
    int fontSizeX;
    int fontSizeY;
};

struct RaceCar {
    int x, y;
    int speed;
    int lane;
    char shape[4][6];
    int color;
    bool crashed;
    int score;
};

struct Obstacle {
    int x, y;
    int type;
    int speed;
    bool active;
    char shape[3][5];
};

struct CompileResult {
    string output;
    bool success;
    double time;
    int errors;
    int warnings;
};

enum ScreenMode {
    DESKTOP, CALCULATOR, FILE_MANAGER, TEXT_EDITOR, HELP,
    SETTINGS, TASK_MANAGER, MEMORY_GAME, PROCESS_MONITOR,
    PAINT, MUSIC_PLAYER, WEATHER, ARCADE_GAME, CLOCK, NEW_YEAR_QUEST,
    MUSIC_EDITOR, RACING_GAME, CPLUS_COMPILER
};
ScreenMode currentScreen = DESKTOP;

enum Direction { UP, DOWN, LEFT, RIGHT };

vector<Calculation> history;
int historyIndex = -1;
const int HISTORY_LIMIT = 20;
int selectedMenuItem = 0;
const int APPS_COUNT = 16;
bool blinkState = false;
clock_t lastBlink = 0;
bool showCursor = true;
clock_t lastCursorToggle = 0;
string currentExpression = "";
int cursorPos = 0;
bool systemMonitorVisible = false;
SystemInfo sysInfo;
map<string, double> variables;
int smileyState = 0;
clock_t lastSmileyChange = 0;
clock_t startupTime = clock();
string paintFileToLoad = "";
vector<App> applications;
int selectedApp = 0;
int appScrollOffset = 0;
vector<FileInfo> fileList;
bool paintFileLoaded = false;
int selectedFile = 0;
string currentDir = "C:\\VirtualDisk";
string textEditorContent = "";
string currentTextFile = "";
bool showHiddenFiles = false;
string commandLine = "";
int commandCursorPos = 0;
vector<string> commandHistory;
int commandHistoryIndex = -1;
const int COMMAND_HISTORY_LIMIT = 50;
bool textEditorMode = false;
int textCursorLine = 0;
int textCursorCol = 0;
int textScrollLine = 0;
vector<string> textLines;
vector<string> textLinesPrev;
bool needRedraw = true;
bool partialRedraw = false;
vector<Task> tasks;
int selectedTask = 0;
bool taskEditMode = false;
bool editingName = true;
string editBuffer = "";
vector<MemoryCard> memoryCards;
int selectedCard = 0;
int firstCard = -1;
int secondCard = -1;
int matchedPairs = 0;
int moves = 0;
bool gameStarted = false;
clock_t gameStartTime;
vector<ProcessInfo> processes;
int selectedProcess = 0;
int processScrollOffset = 0;

vector<vector<PaintPoint>> paintCanvas;
vector<vector<PaintPoint>> paintCanvasPrev;
int paintCursorX = 0;
int paintCursorY = 0;
int currentColorIndex = 1;
ConsoleColor paintColors[10] = {
    BLACK, WHITE, RED, GREEN, BLUE,
    YELLOW, CYAN, MAGENTA, LIGHT_GREEN, LIGHT_CYAN
};
string colorNames[10] = {
    "Black (Erase)", "White", "Red", "Green", "Blue",
    "Yellow", "Cyan", "Magenta", "Light Green", "Light Cyan"
};

int selectedMusic = 0;
int musicScrollOffset = 0;
vector<Song> playlist;
int currentSong = -1;
bool musicPlaying = false;
int musicProgress = 0;

MusicComposition currentMusic;
int musicEditorCursor = 0;
vector<string> musicEditorLines;

WeatherData currentWeather;
int weatherDay = 0;

vector<SnakeSegment> snake;
Direction snakeDirection = RIGHT;
int foodX = 0, foodY = 0;
int score = 0;
int highScore = 0;
bool gameOver = false;
bool gamePaused = false;
int gameSpeed = 150;

ClockData clockData;
time_t lastClockUpdate = 0;

QuestData questData;
vector<string> questSteps;
int questSelectedOption = 0;

SettingsData systemSettings;

RaceCar playerCar;
vector<Obstacle> obstacles;
int roadWidth = 20;
int roadPosition = 30;
int gameDistance = 0;
int raceSpeed = 100;
bool raceOver = false;
int raceScore = 0;
int raceHighScore = 0;
int raceLevel = 1;
int opponentCarX = 40;
int opponentCarY = 10;
int opponentSpeed = 3;

CompileResult lastCompile;
string cppSourceCode;
vector<string> compileHistory;
int selectedCompileHistory = 0;

const int CONSOLE_WIDTH = 120;
const int CONSOLE_HEIGHT = 40;

char screenBuffer[CONSOLE_HEIGHT][CONSOLE_WIDTH];
ConsoleColor fgBuffer[CONSOLE_HEIGHT][CONSOLE_WIDTH];
ConsoleColor bgBuffer[CONSOLE_HEIGHT][CONSOLE_WIDTH];
bool screenDirty[CONSOLE_HEIGHT][CONSOLE_WIDTH];

HQUERY cpuQuery = NULL;
HCOUNTER cpuCounter = NULL;
PDH_FMT_COUNTERVALUE cpuValue;

void initCPUMonitor() {
    PdhOpenQuery(NULL, 0, &cpuQuery);
    PdhAddCounter(cpuQuery, L"\\Processor(_Total)\\% Processor Time", 0, &cpuCounter);
    PdhCollectQueryData(cpuQuery);
}

double getCPUUsage() {
    if (cpuQuery && cpuCounter) {
        PdhCollectQueryData(cpuQuery);
        PDH_FMT_COUNTERVALUE value;
        if (PdhGetFormattedCounterValue(cpuCounter, PDH_FMT_DOUBLE, NULL, &value) == ERROR_SUCCESS) {
            return value.doubleValue;
        }
    }
    return 0.0;
}

void cleanupCPUMonitor() {
    if (cpuCounter) PdhRemoveCounter(cpuCounter);
    if (cpuQuery) PdhCloseQuery(cpuQuery);
}



void playSelectSound() { Beep(800, 50); }
void playMoveSound() { Beep(600, 30); }
void playSuccessSound() { Beep(1000, 100); }
void playErrorSound() { Beep(300, 200); }
void playOpenSound() { Beep(900, 80); Beep(1100, 80); }
void playCloseSound() { Beep(1100, 80); Beep(900, 80); }
void playPaintSound() { Beep(500, 30); }
void playGameSound() { Beep(700, 50); }
void playRaceSound() { Beep(400, 50); }
void playCrashSound() {
    Beep(200, 300);
    Beep(150, 200);
    Beep(100, 300);
}
void playEngineSound() { Beep(50, 30); }
void playBoostSound() { Beep(800, 100); Beep(1000, 80); }
void playCompileSound() { Beep(600, 80); Beep(800, 60); }

void setConsoleEncoding();
void setConsoleSize();
void setFont(const string& fontName, int sizeX, int sizeY);
void setColor(ConsoleColor text, ConsoleColor background);
void setColor(ConsoleColor text);
void resetColor();
void printColored(const string& text, ConsoleColor color, ConsoleColor bg);
void printColored(const string& text, ConsoleColor color);
void printCharColored(char ch, ConsoleColor color, ConsoleColor bg);
void printCharColored(char ch, ConsoleColor color);
void clearScreen();
void clearLine(int y);
void setCursorPos(int x, int y);
void markDirty(int x1, int y1, int x2, int y2);
void markLineDirty(int y);
void updateScreen();
void showBootScreen();
void drawBox(int x, int y, int width, int height, ConsoleColor color, string title, bool shadow);
void drawBox(int x, int y, int width, int height, ConsoleColor color, string title);
void drawBox(int x, int y, int width, int height, ConsoleColor color);
void printHeader();
void printStatusBar(string message, ConsoleColor color);
void printStatusBar(string message);
void getSystemInfo();
void drawSystemPanel();
void drawTaskBar();
void drawDesktop();
void initializeApplications();
bool handleDesktopMode();
int getPrecedence(char op);
double applyOperation(double a, double b, char op);
string replaceVariables(const string& expr);
string replaceFunctions(const string& expr);
double evaluateFunction(char func, double value);
double evaluateExpression(string expr);
bool isValidComplexExpression(string expr);
string toHex(int n);
string toBinary(int n);
void addToHistory(const string& expr, double result);
void printCalculator();
bool handleCalculatorMode();
void printFileManager();
bool handleFileManagerMode();
void printTextEditor();
bool handleTextEditorMode();
void printHelpSystem();
bool handleHelpMode();
void printSettings();
bool handleSettingsMode();
void loadFileList();
string formatSize(long long bytes);
string getFileType(const string& path, bool& isText, bool& isDrawing, bool& isMusic);
void addToCommandHistory(const string& cmd);
bool executeCommand(const string& cmd);
void showCommandHelp();
vector<string> splitCommand(const string& cmd);
bool createFile(const string& name);
bool createFolder(const string& name);
bool renameFileOrFolder(const string& oldName, const string& newName);
bool deleteFileOrFolder(const string& name);
void changeDirectory(const string& dir);
void listAllFiles();
void loadTextFile(const string& filename);
void saveTextFile(const string& filename);
void printTaskManagerApp();
bool handleTaskManagerMode();
void printMemoryGame();
bool handleMemoryGameMode();
void initializeMemoryGame();
void initializeTasks();
void printProcessMonitor();
bool handleProcessMonitorMode();
void loadProcesses();
void printPaintApp();
bool handlePaintMode();
void initializePaintCanvas();
bool savePaintDrawing(const string& filename);
bool loadPaintDrawing(const string& filename);
void printMusicPlayer();
bool handleMusicPlayerMode();
void initializeMusicPlayer();
void playMusicNotes(const vector<int>& notes);
void printWeatherApp();
bool handleWeatherMode();
void initializeWeather();
void updateWeather();
void printArcadeGame();
bool handleArcadeGameMode();
void initializeArcadeGame();
void updateSnake();
void generateFood();
bool checkCollision(int x, int y);
void printClockApp();
bool handleClockMode();
void initializeClock();
void printNewYearQuest();
bool handleNewYearQuestMode();
void initializeNewYearQuest();
void playNewYearSong();
void printMusicEditor();
bool handleMusicEditorMode();
void initializeMusicEditor();
void playMusicComposition();
bool saveMusicComposition(const string& filename);
bool loadMusicComposition(const string& filename);
void printRacingGame();
bool handleRacingGameMode();
void initializeRacingGame();
void updateRacingGame();
void spawnObstacle();
void drawRoad();
void printCPlusCompiler();
bool handleCPlusCompilerMode();
void initializeCompiler();
CompileResult compileCppCode(const string& code);
void runCompiledProgram();
void saveCppCode(const string& filename);
void loadCppCode(const string& filename);
void executeExternalCommand(const string& cmd);

void checkFileExists(const string& path) {
    DWORD attr = GetFileAttributesA(path.c_str());
    if (attr != INVALID_FILE_ATTRIBUTES) {
        printStatusBar("File exists: " + path, LIGHT_GREEN);
    }
    else {
        printStatusBar("File NOT exists: " + path, LIGHT_RED);
    }
}

void playBootMusic() {
    int notes[] = { 262, 294, 330, 349, 392, 440, 494, 523, 494, 440, 392, 349, 330, 392, 494 };
    int durations[] = { 150, 150, 150, 150, 150, 150, 150, 300, 150, 150, 150, 150, 100, 100, 300 };

    for (int i = 0; i < 15; i++) {
        Beep(notes[i], durations[i]);
    }
}

void createVirtualDisk() {
    if (_mkdir("C:\\SOSIOC") != 0 && errno != EEXIST) {
        return;
    }
    if (_mkdir("C:\\SOSIOC\\main") != 0 && errno != EEXIST) {
        return;
    }
    if (_mkdir("C:\\SOSIOC\\main\\drawings") != 0 && errno != EEXIST) {
        return;
    }
    if (_mkdir("C:\\SOSIOC\\main\\music") != 0 && errno != EEXIST) {
        return;
    }
    if (_mkdir("C:\\SOSIOC\\main\\other") != 0 && errno != EEXIST) {
        return;
    }
    if (_mkdir("C:\\SOSIOC\\main\\bin") != 0 && errno != EEXIST) {
        return;
    }
    if (_mkdir("C:\\SOSIOC\\main\\test") != 0 && errno != EEXIST) {
        return;
    }
    if (_mkdir("C:\\SOSIOC\\main\\cpp") != 0 && errno != EEXIST) {
        return;
    }

    ofstream file1("C:\\SOSIOC\\main\\drawings\\example.draw");
    if (file1.is_open()) {
        file1 << "PAINTFILE\n";
        file1 << "Version: 1.0\n";
        file1 << "10 10 1\n";
        file1 << "20 20 4\n";
        file1 << "30 15 2\n";
        file1 << "ENDFILE\n";
        file1.close();
    }

    ofstream file2("C:\\SOSIOC\\main\\music\\demo.music");
    if (file2.is_open()) {
        file2 << "SOSIOC_MUSIC_v1.0\n";
        file2 << "Name: Demo Composition\n";
        file2 << "Notes:\n";
        file2 << "note(262, 500, 100)\n";
        file2 << "note(294, 500, 100)\n";
        file2 << "note(330, 500, 100)\n";
        file2.close();
    }

    ofstream file3("C:\\SOSIOC\\main\\test\\readme.txt");
    if (file3.is_open()) {
        file3 << "Welcome to SOSIOC!\n";
        file3 << "This is a test directory.\n";
        file3 << "You can create your own files here.\n";
        file3.close();
    }

    ofstream file4("C:\\SOSIOC\\main\\cpp\\hello.cpp");
    if (file4.is_open()) {
        file4 << "#include <iostream>\n";
        file4 << "using namespace std;\n\n";
        file4 << "int main() {\n";
        file4 << "    cout << \"Hello from SOSIOC C++ Compiler!\" << endl;\n";
        file4 << "    return 0;\n";
        file4 << "}\n";
        file4.close();
    }

    currentDir = "C:\\SOSIOC\\main";
    SetCurrentDirectoryA("C:\\SOSIOC\\main");
}

bool clearFolder(const string& folderName) {
    string fullPath;
    if (currentDir == ".") {
        char buffer[MAX_PATH];
        GetCurrentDirectoryA(MAX_PATH, buffer);
        fullPath = string(buffer) + "\\" + folderName;
    }
    else {
        fullPath = currentDir + "\\" + folderName;
    }

    DWORD attr = GetFileAttributesA(fullPath.c_str());
    if (attr == INVALID_FILE_ATTRIBUTES) {
        return false;
    }

    if (!(attr & FILE_ATTRIBUTE_DIRECTORY)) {
        return false;
    }

    string searchPath = fullPath + "\\*.*";
    WIN32_FIND_DATAA findData;
    HANDLE hFind = FindFirstFileA(searchPath.c_str(), &findData);

    if (hFind == INVALID_HANDLE_VALUE) {
        return true;
    }

    bool success = true;
    do {
        string fileName = findData.cFileName;
        if (fileName == "." || fileName == "..") {
            continue;
        }

        string filePath = fullPath + "\\" + fileName;

        if (findData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
            if (!clearFolder(folderName + "\\" + fileName)) {
                success = false;
            }
            if (RemoveDirectoryA(filePath.c_str()) == 0) {
                success = false;
            }
        }
        else {
            if (DeleteFileA(filePath.c_str()) == 0) {
                success = false;
            }
        }
    } while (FindNextFileA(hFind, &findData) != 0);

    FindClose(hFind);
    return success;
}

void setConsoleEncoding() {
    SetConsoleOutputCP(866);
    SetConsoleCP(866);
}

void setFont(const string& fontName, int sizeX, int sizeY) {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_FONT_INFOEX fontInfo;
    fontInfo.cbSize = sizeof(fontInfo);
    GetCurrentConsoleFontEx(hConsole, FALSE, &fontInfo);
    wcscpy(fontInfo.FaceName, fontName == "Terminal" ? L"Terminal" : L"Consolas");
    fontInfo.dwFontSize.X = sizeX;
    fontInfo.dwFontSize.Y = sizeY;
    SetCurrentConsoleFontEx(hConsole, FALSE, &fontInfo);
}

void setConsoleSize() {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SMALL_RECT windowSize = { 0, 0, CONSOLE_WIDTH - 1, CONSOLE_HEIGHT - 1 };
    SetConsoleWindowInfo(hConsole, TRUE, &windowSize);
    COORD bufferSize = { CONSOLE_WIDTH, CONSOLE_HEIGHT };
    SetConsoleScreenBufferSize(hConsole, bufferSize);
    HWND consoleWindow = GetConsoleWindow();
    SetWindowLong(consoleWindow, GWL_STYLE,
        GetWindowLong(consoleWindow, GWL_STYLE) & ~WS_MAXIMIZEBOX & ~WS_SIZEBOX);
}

void setColor(ConsoleColor text, ConsoleColor background) {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, (WORD)((background << 4) | text));
}

void setColor(ConsoleColor text) {
    setColor(text, BLACK);
}

void resetColor() {
    setColor(LIGHT_GRAY);
}

void printColored(const string& text, ConsoleColor color, ConsoleColor bg) {
    setColor(color, bg);
    cout << text;
    resetColor();
}

void printColored(const string& text, ConsoleColor color) {
    printColored(text, color, BLACK);
}

void printCharColored(char ch, ConsoleColor color, ConsoleColor bg) {
    setColor(color, bg);
    cout << ch;
    resetColor();
}

void printCharColored(char ch, ConsoleColor color) {
    printCharColored(ch, color, BLACK);
}

void clearScreen() {
    for (int y = 0; y < CONSOLE_HEIGHT; y++) {
        for (int x = 0; x < CONSOLE_WIDTH; x++) {
            screenBuffer[y][x] = ' ';
            fgBuffer[y][x] = LIGHT_GRAY;
            bgBuffer[y][x] = BLACK;
            screenDirty[y][x] = true;
        }
    }
    COORD topLeft = { 0, 0 };
    HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_SCREEN_BUFFER_INFO screen;
    DWORD written;
    GetConsoleScreenBufferInfo(console, &screen);
    DWORD cells = screen.dwSize.X * screen.dwSize.Y;
    FillConsoleOutputCharacterA(console, ' ', cells, topLeft, &written);
    FillConsoleOutputAttribute(console, FOREGROUND_GREEN | FOREGROUND_RED | FOREGROUND_BLUE,
        cells, topLeft, &written);
    SetConsoleCursorPosition(console, topLeft);
}

void clearLine(int y) {
    for (int x = 0; x < CONSOLE_WIDTH; x++) {
        screenBuffer[y][x] = ' ';
        fgBuffer[y][x] = LIGHT_GRAY;
        bgBuffer[y][x] = BLACK;
        screenDirty[y][x] = true;
    }
}

void setCursorPos(int x, int y) {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    COORD pos = { (SHORT)x, (SHORT)y };
    SetConsoleCursorPosition(hConsole, pos);
}

void markDirty(int x1, int y1, int x2, int y2) {
    for (int y = max(0, y1); y <= min(CONSOLE_HEIGHT - 1, y2); y++) {
        for (int x = max(0, x1); x <= min(CONSOLE_WIDTH - 1, x2); x++) {
            screenDirty[y][x] = true;
        }
    }
}

void markLineDirty(int y) {
    if (y >= 0 && y < CONSOLE_HEIGHT) {
        for (int x = 0; x < CONSOLE_WIDTH; x++) {
            screenDirty[y][x] = true;
        }
    }
}

void updateScreen() {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    GetConsoleScreenBufferInfo(hConsole, &csbi);
    WORD defaultAttr = csbi.wAttributes;

    for (int y = 0; y < CONSOLE_HEIGHT; y++) {
        for (int x = 0; x < CONSOLE_WIDTH; x++) {
            if (screenDirty[y][x]) {
                COORD pos = { (SHORT)x, (SHORT)y };
                SetConsoleCursorPosition(hConsole, pos);

                WORD attr = (WORD)((bgBuffer[y][x] << 4) | fgBuffer[y][x]);
                SetConsoleTextAttribute(hConsole, attr);

                cout << screenBuffer[y][x];

                screenDirty[y][x] = false;
            }
        }
    }

    COORD cursorPos = { 0, (SHORT)(CONSOLE_HEIGHT - 1) };
    SetConsoleCursorPosition(hConsole, cursorPos);
    SetConsoleTextAttribute(hConsole, defaultAttr);
}

void putChar(int x, int y, char ch, ConsoleColor fg, ConsoleColor bg) {
    if (x >= 0 && x < CONSOLE_WIDTH && y >= 0 && y < CONSOLE_HEIGHT) {
        if (screenBuffer[y][x] != ch || fgBuffer[y][x] != fg || bgBuffer[y][x] != bg) {
            screenBuffer[y][x] = ch;
            fgBuffer[y][x] = fg;
            bgBuffer[y][x] = bg;
            screenDirty[y][x] = true;
        }
    }
}

void putString(int x, int y, const string& str, ConsoleColor fg, ConsoleColor bg) {
    for (size_t i = 0; i < str.length(); i++) {
        putChar(x + i, y, str[i], fg, bg);
    }
}

void showBootScreen() {
    clearScreen();

    for (int i = 0; i < CONSOLE_WIDTH; i++) {
        putChar(i, 2, char(176), LIGHT_GRAY, BLACK);
    }

    struct LogoData {
        const char* lines[6];
    };

    LogoData logoData = {
        {
            "   ███████   █████   ███████  ██████   █████    █████ ",
            "   ██       ███████  ██         ██    ███████  ███████   ",
            "   ███████  ██   ██  ███████    ██    ██   ██  ██     ",
            "        ██  ██   ██       ██    ██    ██   ██  ██     ",
            "   ███████  ███████  ███████    ██    ███████  ███████ ",
            "   ███████   █████   ███████  ██████   █████    █████ "
        }
    };

    DWORD threadId;
    HANDLE logoThread = CreateThread(NULL, 0,
        [](LPVOID param) -> DWORD {
            LogoData* data = (LogoData*)param;
            for (int line = 0; line < 6; line++) {
                const char* lineText = data->lines[line];
                for (int i = 0; i < 56; i++) {
                    if (lineText[i] == '█') {
                        int colorIdx = (line + i) % 3;
                        ConsoleColor color = LIGHT_CYAN;
                        if (colorIdx == 1) color = CYAN;
                        else if (colorIdx == 2) color = LIGHT_BLUE;
                        putChar(25 + i, 4 + line, char(178), color, BLACK);
                    }
                    else {
                        putChar(25 + i, 4 + line, lineText[i], LIGHT_GRAY, BLACK);
                    }
                }
                updateScreen();
                Sleep(50);
            }
            return 0;
        },
        &logoData, 0, &threadId);

    putString(52, 11, "v1.0 - Developer Edition", YELLOW, BLACK);
    putString(40, 13, "Simple Calculator", LIGHT_CYAN, BLACK);

    putChar(30, 14, char(204), LIGHT_GRAY, BLACK);
    for (int i = 1; i < 59; i++) {
        putChar(30 + i, 14, char(205), LIGHT_GRAY, BLACK);
    }
    putChar(30 + 59, 14, char(185), LIGHT_GRAY, BLACK);

    updateScreen();
    Sleep(100);

    WaitForSingleObject(logoThread, INFINITE);
    CloseHandle(logoThread);

    putString(40, 16, "Initializing virtual disk...", LIGHT_GRAY, BLACK);
    updateScreen();
    createVirtualDisk();

    HANDLE musicThread = CreateThread(NULL, 0,
        [](LPVOID) -> DWORD {
            int notes[] = { 262, 294, 330, 349, 392, 440, 494, 523, 494, 440, 392, 349, 330, 392, 494 };
            int durations[] = { 150, 150, 150, 150, 150, 150, 150, 300, 150, 150, 150, 150, 100, 100, 300 };

            for (int i = 0; i < 15; i++) {
                Beep(notes[i], durations[i]);
            }
            return 0;
        }, NULL, 0, &threadId);

    const char* spinner = "|/-\\";
    int spinnerIndex = 0;

    const char* messages[] = {
        "Checking system integrity...",
        "Loading kernel modules...",
        "Mounting filesystems...",
        "Starting system services...",
        "Initializing display driver...",
        "Preparing desktop environment...",
        "Loading user preferences...",
        "Almost ready..."
    };

    for (int progress = 0; progress <= 100; progress += 2) {
        int y = 18;

        for (int line = 0; line < 3; line++) {
            for (int x = 30; x < 90; x++) {
                putChar(x, y + line, ' ', LIGHT_GRAY, BLACK);
            }
        }

        putChar(30, y, char(204), LIGHT_GRAY, BLACK);
        putChar(31, y, spinner[spinnerIndex % 4], LIGHT_GREEN, BLACK);
        spinnerIndex++;
        putChar(32, y, char(185), LIGHT_GRAY, BLACK);

        int msgIndex = min(progress / 12, 7);
        putString(35, y, messages[msgIndex], LIGHT_GRAY, BLACK);

        putChar(30, y + 2, char(204), LIGHT_GRAY, BLACK);

        int barWidth = 50;
        int filled = (progress * barWidth) / 100;

        for (int i = 0; i < barWidth; i++) {
            if (i < filled) {
                ConsoleColor color = RED;
                if (i >= 15) color = YELLOW;
                if (i >= 30) color = GREEN;
                if (i >= 45) color = LIGHT_GREEN;
                putChar(31 + i, y + 2, char(219), color, color);
            }
            else {
                putChar(31 + i, y + 2, char(176), DARK_GRAY, DARK_GRAY);
            }
        }

        putChar(31 + barWidth, y + 2, char(185), LIGHT_GRAY, BLACK);

        string percent = " " + to_string(progress) + "%";
        putString(32 + barWidth, y + 2, percent, LIGHT_GRAY, BLACK);

        const char* sysMsg = "Allocating memory...";
        if (progress >= 20) sysMsg = "CPU initialization...";
        if (progress >= 40) sysMsg = "RAM detection...";
        if (progress >= 60) sysMsg = "VGA setup...";
        if (progress >= 80) sysMsg = "Finalizing...";

        putString(31, y + 4, sysMsg, LIGHT_GRAY, BLACK);

        updateScreen();
        Sleep(80);

        if (progress > 90) Sleep(20);
    }

    WaitForSingleObject(musicThread, INFINITE);
    CloseHandle(musicThread);

    putString(33, 18, "✓ System initialization complete!", LIGHT_GREEN, BLACK);

    putChar(30, 20, char(204), LIGHT_GREEN, BLACK);
    for (int i = 0; i < 50; i++) {
        putChar(31 + i, 20, char(219), LIGHT_GREEN, LIGHT_GREEN);
    }
    putChar(31 + 50, 20, char(185), LIGHT_GREEN, BLACK);

    Beep(523, 200);
    Beep(659, 200);
    Beep(784, 400);

    for (int i = 0; i < 58; i++) {
        putChar(31 + i, 25, char(205), LIGHT_GRAY, BLACK);
    }

    putString(35, 27, "SOSIOC Operating System v1.0", LIGHT_CYAN, BLACK);
    putString(32, 28, "16 built-in applications", LIGHT_GRAY, BLACK);
    putString(32, 29, "C:\\SOSIOC\\main", LIGHT_GRAY, BLACK);

    updateScreen();

    for (int i = 0; i < 6; i++) {
        putString(38, 32, "Press any key to enter desktop ", YELLOW, BLACK);
        updateScreen();
        Sleep(400);

        for (int j = 0; j < 35; j++) {
            putChar(38 + j, 32, ' ', YELLOW, BLACK);
        }
        updateScreen();
        Sleep(400);
    }

    putString(38, 32, "Press any key to enter desktop ", YELLOW, BLACK);
    updateScreen();

    _getch();
}

void drawBox(int x, int y, int width, int height, ConsoleColor color, string title, bool shadow) {
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            char ch = ' ';
            if (i == 0 && j == 0) ch = char(218);
            else if (i == 0 && j == width - 1) ch = char(191);
            else if (i == height - 1 && j == 0) ch = char(192);
            else if (i == height - 1 && j == width - 1) ch = char(217);
            else if (i == 0 || i == height - 1) ch = char(196);
            else if (j == 0 || j == width - 1) ch = char(179);
            putChar(x + j, y + i, ch, color, BLACK);
        }
    }

    if (!title.empty()) {
        string displayTitle = " " + title + " ";
        putString(x + 2, y, displayTitle, LIGHT_CYAN, color);
    }

    if (shadow) {
        for (int i = 1; i < height; i++) {
            putChar(x + width, y + i, char(219), DARK_GRAY, DARK_GRAY);
        }
        for (int i = 0; i < width + 1; i++) {
            putChar(x + i, y + height, char(219), DARK_GRAY, DARK_GRAY);
        }
    }
}

void drawBox(int x, int y, int width, int height, ConsoleColor color, string title) {
    drawBox(x, y, width, height, color, title, false);
}

void drawBox(int x, int y, int width, int height, ConsoleColor color) {
    drawBox(x, y, width, height, color, "", false);
}

void printHeader() {
    static string lastDateStr = "";
    time_t now = time(0);
    tm* ltm = localtime(&now);
    char dateStr[30];
    strftime(dateStr, sizeof(dateStr), "%d.%m.%Y %H:%M:%S", ltm);

    bool headerChanged = false;

    if (lastDateStr != dateStr) {
        lastDateStr = dateStr;
        headerChanged = true;
    }

    if (clock() - lastSmileyChange > 1000) {
        smileyState = (smileyState + 1) % 4;
        lastSmileyChange = clock();
        headerChanged = true;
    }

    if (headerChanged) {
        for (int i = 0; i < CONSOLE_WIDTH; i++) {
            putChar(i, 0, ' ', WHITE, BLUE);
        }

        char smiley;
        switch (smileyState) {
        case 0: smiley = 1; break;
        case 1: smiley = 2; break;
        case 2: smiley = 1; break;
        case 3: smiley = 2; break;
        }

        putChar(1, 0, smiley, YELLOW, BLUE);
        putChar(2, 0, ' ', WHITE, BLUE);
        putString(3, 0, "SOSIOC v1.0 - Desktop Environment", WHITE, BLUE);

        putString(CONSOLE_WIDTH - strlen(dateStr) - 1, 0, dateStr, YELLOW, BLUE);

        for (int i = 0; i < CONSOLE_WIDTH; i++) {
            putChar(i, 2, char(205), LIGHT_GRAY, BLACK);
        }
    }
}

void printStatusBar(string message, ConsoleColor color) {
    static string lastMessage = "";
    static ConsoleColor lastColor = LIGHT_GRAY;

    if (message != lastMessage || color != lastColor) {
        lastMessage = message;
        lastColor = color;

        for (int i = 0; i < CONSOLE_WIDTH; i++) {
            putChar(i, CONSOLE_HEIGHT - 1, ' ', LIGHT_GRAY, DARK_GRAY);
        }

        putString(1, CONSOLE_HEIGHT - 1, "Status:", YELLOW, DARK_GRAY);
        putString(9, CONSOLE_HEIGHT - 1, message.substr(0, CONSOLE_WIDTH - 80), color, DARK_GRAY);
        putString(CONSOLE_WIDTH - 15, CONSOLE_HEIGHT - 1, "Ctrl+H", YELLOW, DARK_GRAY);
    }
}

void printStatusBar(string message) {
    printStatusBar(message, LIGHT_GRAY);
}

void getSystemInfo() {
    static clock_t lastUpdate = 0;
    clock_t now = clock();
    if (now - lastUpdate < 1000) return;
    lastUpdate = now;

    static MEMORYSTATUSEX memInfo;
    memInfo.dwLength = sizeof(MEMORYSTATUSEX);
    GlobalMemoryStatusEx(&memInfo);

    PROCESS_MEMORY_COUNTERS pmc;
    GetProcessMemoryInfo(GetCurrentProcess(), &pmc, sizeof(pmc));

    sysInfo.cpuLoad = getCPUUsage();
    sysInfo.totalMemory = memInfo.ullTotalPhys / (1024 * 1024);
    sysInfo.freeMemory = memInfo.ullAvailPhys / (1024 * 1024);
    sysInfo.memoryUsage = 100 - (int)((memInfo.ullAvailPhys * 100) / memInfo.ullTotalPhys);
    sysInfo.appMemory = pmc.WorkingSetSize / (1024.0 * 1024.0);

    DWORD processes[1024];
    DWORD needed;
    if (EnumProcesses(processes, sizeof(processes), &needed)) {
        sysInfo.processCount = needed / sizeof(DWORD);
    }

    clock_t uptimeMs = clock() - startupTime;
    int seconds = (int)(uptimeMs / CLOCKS_PER_SEC);
    int hours = seconds / 3600;
    int minutes = (seconds % 3600) / 60;
    seconds = seconds % 60;
    sysInfo.uptime = to_string(hours) + "h " + to_string(minutes) + "m " + to_string(seconds) + "s";
}

void drawSystemPanel() {
    if (!systemMonitorVisible) {
        int panelX = CONSOLE_WIDTH - 38;
        int panelY = 3;
        int panelWidth = 36;
        int panelHeight = 20;

        for (int y = panelY; y < panelY + panelHeight; y++) {
            for (int x = panelX; x < panelX + panelWidth; x++) {
                putChar(x, y, ' ', LIGHT_GRAY, BLACK);
            }
        }
        return;
    }

    static string lastCPUStr = "";
    static string lastRAMStr = "";
    static string lastUptime = "";

    string cpuStr = to_string((int)sysInfo.cpuLoad) + "%";
    string ramStr = to_string(sysInfo.memoryUsage) + "%";

    bool needsUpdate = (cpuStr != lastCPUStr || ramStr != lastRAMStr || sysInfo.uptime != lastUptime);

    if (!needsUpdate) return;

    lastCPUStr = cpuStr;
    lastRAMStr = ramStr;
    lastUptime = sysInfo.uptime;

    int panelX = CONSOLE_WIDTH - 38;
    int panelY = 3;
    int panelWidth = 36;
    int panelHeight = 20;

    drawBox(panelX, panelY, panelWidth, panelHeight, MAGENTA, "SYS MONITOR", true);

    putChar(panelX + 1, panelY + 2, char(218), LIGHT_CYAN, BLACK);
    for (int i = 0; i < panelWidth - 2; i++) putChar(panelX + 2 + i, panelY + 2, char(196), LIGHT_CYAN, BLACK);
    putChar(panelX + panelWidth - 1, panelY + 2, char(191), LIGHT_CYAN, BLACK);

    putChar(panelX + 1, panelY + 4, char(218), LIGHT_CYAN, BLACK);
    for (int i = 0; i < panelWidth - 2; i++) putChar(panelX + 2 + i, panelY + 4, char(196), LIGHT_CYAN, BLACK);
    putChar(panelX + panelWidth - 1, panelY + 4, char(191), LIGHT_CYAN, BLACK);

    putString(panelX + 2, panelY + 1, char(175) + string(" SYSTEM STATUS"), LIGHT_GREEN, BLACK);

    putString(panelX + 2, panelY + 3, "CPU: ", YELLOW, BLACK);
    int cpuBarWidth = 20;
    int cpuFilled = (sysInfo.cpuLoad * cpuBarWidth) / 100;
    putString(panelX + 7, panelY + 3, "[", LIGHT_GRAY, BLACK);
    for (int i = 0; i < cpuBarWidth; i++) {
        if (i < cpuFilled) {
            ConsoleColor barColor = LIGHT_GREEN;
            if (sysInfo.cpuLoad > 70) barColor = LIGHT_RED;
            else if (sysInfo.cpuLoad > 40) barColor = YELLOW;
            putChar(panelX + 8 + i, panelY + 3, char(219), barColor, barColor);
        }
        else {
            putChar(panelX + 8 + i, panelY + 3, char(176), DARK_GRAY, BLACK);
        }
    }
    putString(panelX + 8 + cpuBarWidth, panelY + 3, "] " + cpuStr, LIGHT_GRAY, BLACK);

    putString(panelX + 2, panelY + 5, "RAM: ", YELLOW, BLACK);
    int ramBarWidth = 20;
    int ramFilled = (sysInfo.memoryUsage * ramBarWidth) / 100;
    putString(panelX + 7, panelY + 5, "[", LIGHT_GRAY, BLACK);
    for (int i = 0; i < ramBarWidth; i++) {
        if (i < ramFilled) {
            ConsoleColor barColor = LIGHT_CYAN;
            if (sysInfo.memoryUsage > 80) barColor = LIGHT_RED;
            else if (sysInfo.memoryUsage > 60) barColor = YELLOW;
            putChar(panelX + 8 + i, panelY + 5, char(219), barColor, barColor);
        }
        else {
            putChar(panelX + 8 + i, panelY + 5, char(176), DARK_GRAY, BLACK);
        }
    }
    putString(panelX + 8 + ramBarWidth, panelY + 5, "] " + ramStr, LIGHT_GRAY, BLACK);

    putChar(panelX + 1, panelY + 7, char(195), LIGHT_CYAN, BLACK);
    for (int i = 0; i < panelWidth - 2; i++) putChar(panelX + 2 + i, panelY + 7, char(196), LIGHT_CYAN, BLACK);
    putChar(panelX + panelWidth - 1, panelY + 7, char(180), LIGHT_CYAN, BLACK);

    putString(panelX + 2, panelY + 8, "MEMORY INFO", LIGHT_GREEN, BLACK);
    putString(panelX + 2, panelY + 9, "Total: " + to_string(sysInfo.totalMemory) + " MB", LIGHT_GRAY, BLACK);
    putString(panelX + 2, panelY + 10, "Free:  " + to_string(sysInfo.freeMemory) + " MB", LIGHT_GRAY, BLACK);
    putString(panelX + 2, panelY + 11, "App:   " + to_string(sysInfo.appMemory).substr(0, 4) + " MB", LIGHT_GRAY, BLACK);

    putChar(panelX + 1, panelY + 13, char(195), LIGHT_CYAN, BLACK);
    for (int i = 0; i < panelWidth - 2; i++) putChar(panelX + 2 + i, panelY + 13, char(196), LIGHT_CYAN, BLACK);
    putChar(panelX + panelWidth - 1, panelY + 13, char(180), LIGHT_CYAN, BLACK);

    putString(panelX + 2, panelY + 14, "SYSTEM INFO", LIGHT_GREEN, BLACK);
    putString(panelX + 2, panelY + 15, "Processes: " + to_string(sysInfo.processCount), LIGHT_GRAY, BLACK);
    putString(panelX + 2, panelY + 16, "Uptime: " + sysInfo.uptime, LIGHT_GRAY, BLACK);
    putString(panelX + 2, panelY + 17, "Apps: " + to_string(applications.size()), LIGHT_GRAY, BLACK);

    time_t now = time(0);
    tm* ltm = localtime(&now);
    char timeStr[9];
    strftime(timeStr, sizeof(timeStr), "%H:%M:%S", ltm);

    putChar(panelX + 1, panelY + 19, char(192), LIGHT_CYAN, BLACK);
    for (int i = 0; i < panelWidth - 2; i++) putChar(panelX + 2 + i, panelY + 19, char(196), LIGHT_CYAN, BLACK);
    putChar(panelX + panelWidth - 1, panelY + 19, char(217), LIGHT_CYAN, BLACK);

    putString(panelX + 2, panelY + 18, "Time: " + string(timeStr), LIGHT_YELLOW, BLACK);

    needRedraw = true;
}

void drawTaskBar() {
    static int lastSelectedApp = -1;
    static string lastCPUStr = "";
    static string lastRAMStr = "";

    string cpuStr = to_string((int)sysInfo.cpuLoad) + "%";
    string ramStr = to_string(sysInfo.memoryUsage) + "%";

    bool needsUpdate = (lastSelectedApp != selectedApp || lastCPUStr != cpuStr || lastRAMStr != ramStr);

    if (!needsUpdate) return;

    lastSelectedApp = selectedApp;
    lastCPUStr = cpuStr;
    lastRAMStr = ramStr;

    int taskbarY = CONSOLE_HEIGHT - 6;

    for (int i = 0; i < CONSOLE_WIDTH; i++) {
        putChar(i, taskbarY, char(205), LIGHT_GRAY, DARK_GRAY);
    }

    for (int line = 0; line < 4; line++) {
        for (int i = 0; i < CONSOLE_WIDTH; i++) {
            putChar(i, taskbarY + 1 + line, ' ', WHITE, BLUE);
        }
    }

    putString(1, taskbarY + 1, "Start", LIGHT_GREEN, BLUE);

    int maxAppsPerLine = 12;
    int totalLines = (applications.size() + maxAppsPerLine - 1) / maxAppsPerLine;

    for (int line = 0; line < totalLines; line++) {
        int xPos = 7;
        int startIdx = line * maxAppsPerLine;
        int endIdx = min(startIdx + maxAppsPerLine, (int)applications.size());

        for (int i = startIdx; i < endIdx; i++) {
            string appName = applications[i].name.substr(0, 8);
            if (i == selectedApp) {
                putString(xPos, taskbarY + 1 + line, " " + appName + " ", YELLOW, LIGHT_BLUE);
            }
            else {
                putString(xPos, taskbarY + 1 + line, " " + appName + " ", WHITE, BLUE);
            }
            xPos += (int)appName.length() + 3;
        }
    }

    putString(CONSOLE_WIDTH - 20, taskbarY + 1, "CPU: " + cpuStr, LIGHT_GREEN, BLUE);
    putString(CONSOLE_WIDTH - 20, taskbarY + 2, "RAM: " + ramStr, LIGHT_CYAN, BLUE);

    for (int i = 0; i < CONSOLE_WIDTH; i++) {
        putChar(i, taskbarY + 5, char(205), LIGHT_GRAY, DARK_GRAY);
    }
}

void initializeApplications() {
    applications.clear();
    App app1 = { 1, "Calculator", "Advanced calculator with history" };
    App app2 = { 2, "File Manager", "Browse and manage files" };
    App app3 = { 3, "Text Editor", "Simple text editor" };
    App app4 = { 4, "System Settings", "Configure system options" };
    App app5 = { 5, "Task Manager", "Manage your tasks" };
    App app6 = { 6, "Memory Game", "Test your memory" };
    App app7 = { 7, "Process Monitor", "Monitor system processes" };
    App app8 = { 8, "Paint", "Simple drawing program" };
    App app9 = { 9, "Music Player", "Play music files" };
    App app10 = { 10, "Weather", "Weather forecast" };
    App app11 = { 11, "Arcade Game", "Snake arcade game" };
    App app12 = { 12, "Clock", "Digital and analog clock with alarm" };
    App app13 = { 13, "New Year Quest", "Holiday adventure game" };
    App app14 = { 14, "Music Editor", "Create and edit music compositions" };
    App app15 = { 15, "Racing Game", "Arcade racing game" };
    App app16 = { 16, "C++ Compiler", "Compile and run C++ code" };
    applications.push_back(app1);
    applications.push_back(app2);
    applications.push_back(app3);
    applications.push_back(app4);
    applications.push_back(app5);
    applications.push_back(app6);
    applications.push_back(app7);
    applications.push_back(app8);
    applications.push_back(app9);
    applications.push_back(app10);
    applications.push_back(app11);
    applications.push_back(app12);
    applications.push_back(app13);
    applications.push_back(app14);
    applications.push_back(app15);
    applications.push_back(app16);
}

void initializeTasks() {
    tasks.clear();
    Task task1 = { 1, "Learn C++", "Complete SOSIOC project", false };
    Task task2 = { 2, "Fix bugs", "Fix all system bugs", false };
    Task task3 = { 3, "Add features", "Add new applications", false };
    Task task4 = { 4, "Test system", "Test all modules", false };
    Task task5 = { 5, "Write docs", "Document the code", false };
    tasks.push_back(task1);
    tasks.push_back(task2);
    tasks.push_back(task3);
    tasks.push_back(task4);
    tasks.push_back(task5);
}

void initializeMemoryGame() {
    memoryCards.clear();
    selectedCard = 0;
    firstCard = -1;
    secondCard = -1;
    matchedPairs = 0;
    moves = 0;
    gameStarted = false;
    gameStartTime = clock();
    vector<int> values;
    for (int i = 1; i <= 8; i++) {
        values.push_back(i);
        values.push_back(i);
    }
    random_device rd;
    mt19937 g(rd());
    shuffle(values.begin(), values.end(), g);
    for (int i = 0; i < 16; i++) {
        MemoryCard card;
        card.value = values[i];
        card.revealed = false;
        card.matched = false;
        card.x = 20 + (i % 8) * 10;
        card.y = 10 + (i / 8) * 4;
        memoryCards.push_back(card);
    }
}

void initializePaintCanvas() {
    paintFileLoaded = false;
    if (paintCanvas.size() != 30) {
        paintCanvas.resize(30);
        paintCanvasPrev.resize(30);
    }

    for (int i = 0; i < 30; i++) {
        if (paintCanvas[i].size() != 80) {
            paintCanvas[i].resize(80);
            paintCanvasPrev[i].resize(80);
        }
        for (int j = 0; j < 80; j++) {
            paintCanvas[i][j].ch = ' ';
            paintCanvas[i][j].color = BLACK;
            paintCanvas[i][j].active = false;
            paintCanvasPrev[i][j] = paintCanvas[i][j];
        }
    }
    paintCursorX = 40;
    paintCursorY = 15;
    currentColorIndex = 1;
}

void initializeMusicPlayer() {
    playlist.clear();
    Song song1;
    song1.title = "Simple Melody";
    song1.artist = "SOSIOC Composer";
    song1.duration = 30;
    song1.notes = { 262, 294, 330, 349, 392 };
    Song song2;
    song2.title = "Happy Tune";
    song2.artist = "System Band";
    song2.duration = 40;
    song2.notes = { 392, 440, 494, 523, 587 };
    Song song3;
    song3.title = "Sad Song";
    song3.artist = "Blue Orchestra";
    song3.duration = 35;
    song3.notes = { 220, 196, 175, 165, 147 };
    Song song4;
    song4.title = "March";
    song4.artist = "Digital Marching Band";
    song4.duration = 45;
    song4.notes = { 330, 330, 349, 392, 392 };
    playlist.push_back(song1);
    playlist.push_back(song2);
    playlist.push_back(song3);
    playlist.push_back(song4);
    selectedMusic = 0;
    currentSong = -1;
    musicPlaying = false;
    musicProgress = 0;
}

void initializeMusicEditor() {
    currentMusic.name = "New Composition";
    currentMusic.notes.clear();
    currentMusic.isPlaying = false;
    currentMusic.currentNote = 0;
    musicEditorLines.clear();
    musicEditorLines.push_back("// Music Composition");
    musicEditorLines.push_back("// Format: note(pitch, duration, delay)");
    musicEditorLines.push_back("// Example: note(440, 500, 100)");
    musicEditorLines.push_back("");
    musicEditorLines.push_back("note(262, 500, 100)");
    musicEditorLines.push_back("note(294, 500, 100)");
    musicEditorLines.push_back("note(330, 500, 100)");
    musicEditorLines.push_back("note(349, 500, 100)");
    musicEditorLines.push_back("note(392, 500, 100)");
    musicEditorLines.push_back("note(440, 500, 100)");
    musicEditorLines.push_back("note(494, 500, 100)");
    musicEditorLines.push_back("note(523, 1000, 200)");
    musicEditorCursor = 0;
}

void initializeWeather() {
    currentWeather.city = "Moscow";
    currentWeather.temperature = 15.5;
    currentWeather.humidity = 65.0;
    currentWeather.condition = "Partly Cloudy";
    currentWeather.windSpeed = 12.3;
    currentWeather.forecast = "Clear skies tomorrow";
    weatherDay = 0;
}

void initializeArcadeGame() {
    snake.clear();
    SnakeSegment head;
    head.x = 40;
    head.y = 15;
    snake.push_back(head);
    snakeDirection = RIGHT;
    score = 0;
    gameOver = false;
    gamePaused = false;
    gameSpeed = 150;
    generateFood();
}

void initializeClock() {
    clockData.showAnalog = true;
    clockData.alarmSet = false;
    clockData.alarmHour = 7;
    clockData.alarmMinute = 30;
    clockData.timerSeconds = 300;
    clockData.timerRunning = false;
    clockData.timerStart = time(0);
}

void initializeNewYearQuest() {
    questData.currentStep = 0;
    questData.hasKey = false;
    questData.hasGift = false;
    questData.metSanta = false;
    questData.puzzleSolved = false;
    questSteps.clear();
    questSteps.push_back("You wake up on December 31st. The house is decorated for New Year.");
    questSteps.push_back("You need to find the hidden gifts before midnight.");
    questSteps.push_back("You see three doors: Living Room (1), Kitchen (2), Attic (3).");
    questSteps.push_back("In the Living Room, you find a Christmas tree with a puzzle.");
    questSteps.push_back("Solve the puzzle: What has 4 letters, sometimes 9, but never 5?");
    questSteps.push_back("You find a key under the tree after solving the puzzle.");
    questSteps.push_back("In the Kitchen, you find cookies and milk left for Santa.");
    questSteps.push_back("You hear noise from the Attic. Use the key to unlock it.");
    questSteps.push_back("In the Attic, you meet Santa Claus with your gifts!");
    questSteps.push_back("You receive your gifts and celebrate the New Year!");
}

void initializeRacingGame() {
    playerCar.x = 40;
    playerCar.y = 30;
    playerCar.speed = 0;
    playerCar.lane = 1;
    playerCar.color = LIGHT_CYAN;
    playerCar.crashed = false;
    playerCar.score = 0;

    const char* carShape[4][6] = {
        {"  ██ "},
        {"██████"},
        {"██████"},
        {"  ██  "}
    };

    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 6; j++) {
            playerCar.shape[i][j] = carShape[i][0][j];
        }
    }

    obstacles.clear();
    gameDistance = 0;
    raceOver = false;
    raceScore = 0;
    raceLevel = 1;
    raceSpeed = 100;
    opponentCarX = 40;
    opponentCarY = 10;
    opponentSpeed = 3;

    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> dis(0, 2);

    for (int i = 0; i < 5; i++) {
        Obstacle obs;
        obs.type = dis(gen);
        obs.x = roadPosition + dis(gen) * 7;
        obs.y = -10 - i * 15;
        obs.speed = 2 + raceLevel;
        obs.active = true;

        const char* shapes[3][3] = {
            {"▓▓▓", "▓▓▓", "▓▓▓"},
            {"▒▒▒", "▒▒▒", "▒▒▒"},
            {"░░░", "░░░", "░░░"}
        };

        for (int j = 0; j < 3; j++) {
            for (int k = 0; k < 3; k++) {
                obs.shape[j][k] = shapes[obs.type][j][k];
            }
        }

        obstacles.push_back(obs);
    }
}

void initializeCompiler() {
    cppSourceCode = "#include <iostream>\nusing namespace std;\n\nint main() {\n    cout << \"Hello from SOSIOC C++ Compiler!\" << endl;\n    return 0;\n}";
    lastCompile.success = false;
    lastCompile.output = "";
    lastCompile.time = 0;
    lastCompile.errors = 0;
    lastCompile.warnings = 0;
    compileHistory.clear();
    compileHistory.push_back("Sample program compiled successfully");
    compileHistory.push_back("Error: missing semicolon on line 5");
    compileHistory.push_back("Compilation time: 0.45 seconds");
    selectedCompileHistory = 0;
}

void playNewYearSong() {
    Beep(247, 500);
    Beep(417, 500);
    Beep(417, 500);
    Beep(370, 500);
    Beep(417, 500);
    Beep(329, 500);
    Beep(247, 500);
    Beep(247, 500);
    Beep(247, 500);
    Beep(417, 500);
    Beep(417, 500);
    Beep(370, 500);
    Beep(417, 500);
    Beep(497, 500);
    Sleep(500);
    Beep(497, 500);
    Beep(277, 500);
    Beep(277, 500);
    Beep(440, 500);
    Beep(440, 500);
    Beep(417, 500);
    Beep(370, 500);
    Beep(329, 500);
    Beep(247, 500);
    Beep(417, 500);
    Beep(417, 500);
    Beep(370, 500);
    Beep(417, 500);
    Beep(329, 500);
}

void generateFood() {
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> disX(2, 77);
    uniform_int_distribution<> disY(5, 34);
    do {
        foodX = disX(gen);
        foodY = disY(gen);
    } while (checkCollision(foodX, foodY));
}

bool checkCollision(int x, int y) {
    if (x < 2 || x > 77 || y < 5 || y > 34) {
        return true;
    }
    for (const auto& segment : snake) {
        if (segment.x == x && segment.y == y) {
            return true;
        }
    }
    return false;
}

void updateSnake() {
    if (gameOver || gamePaused) return;
    int headX = snake[0].x;
    int headY = snake[0].y;
    switch (snakeDirection) {
    case UP: headY--; break;
    case DOWN: headY++; break;
    case LEFT: headX--; break;
    case RIGHT: headX++; break;
    }
    if (checkCollision(headX, headY)) {
        gameOver = true;
        if (score > highScore) {
            highScore = score;
        }
        return;
    }
    SnakeSegment newHead;
    newHead.x = headX;
    newHead.y = headY;
    snake.insert(snake.begin(), newHead);
    if (headX == foodX && headY == foodY) {
        score += 10;
        if (score % 50 == 0 && gameSpeed > 50) {
            gameSpeed -= 10;
        }
        generateFood();
        Beep(800, 100);
    }
    else {
        snake.pop_back();
    }
}

void updateWeather() {
    static int counter = 0;
    counter++;
    currentWeather.temperature = 10.0 + 10.0 * sin(counter * 0.1);
    currentWeather.humidity = 50.0 + 20.0 * sin(counter * 0.05);
    currentWeather.windSpeed = 5.0 + 10.0 * sin(counter * 0.07);
    int cond = (counter / 10) % 4;
    switch (cond) {
    case 0: currentWeather.condition = "Sunny"; break;
    case 1: currentWeather.condition = "Cloudy"; break;
    case 2: currentWeather.condition = "Rainy"; break;
    case 3: currentWeather.condition = "Windy"; break;
    }
    weatherDay = (counter / 20) % 7;
}

void playMusicNotes(const vector<int>& notes) {
    for (int note : notes) {
        Beep(note, 300);
        Sleep(50);
    }
}

void playMusicComposition() {
    for (const auto& note : currentMusic.notes) {
        if (note.pitch > 0) {
            Beep(note.pitch, note.duration);
        }
        Sleep(note.delay);
    }
}

void spawnObstacle() {
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> disType(0, 2);
    uniform_int_distribution<> disLane(0, 2);

    Obstacle obs;
    obs.type = disType(gen);
    obs.x = roadPosition + disLane(gen) * 7;
    obs.y = -5;
    obs.speed = 2 + raceLevel;
    obs.active = true;

    const char* shapes[3][3] = {
        {"▓▓▓", "▓▓▓", "▓▓▓"},
        {"▒▒▒", "▒▒▒", "▒▒▒"},
        {"░░░", "░░░", "░░░"}
    };

    for (int j = 0; j < 3; j++) {
        for (int k = 0; k < 3; k++) {
            obs.shape[j][k] = shapes[obs.type][j][k];
        }
    }

    obstacles.push_back(obs);
}

void updateRacingGame() {
    if (raceOver) return;

    gameDistance++;
    raceScore = gameDistance / 10;

    if (gameDistance % 500 == 0) {
        raceLevel++;
        if (raceSpeed > 30) raceSpeed -= 10;
        playBoostSound();
    }

    opponentCarY += opponentSpeed;
    if (opponentCarY > 40) {
        opponentCarY = -10;
        random_device rd;
        mt19937 gen(rd());
        uniform_int_distribution<> dis(roadPosition, roadPosition + roadWidth - 6);
        opponentCarX = dis(gen);
    }

    for (auto& obs : obstacles) {
        obs.y += obs.speed;

        if (obs.y > 40) {
            obs.y = -10;
            random_device rd;
            mt19937 gen(rd());
            uniform_int_distribution<> dis(0, 2);
            obs.x = roadPosition + dis(gen) * 7;
        }

        if (obs.y >= playerCar.y - 3 && obs.y <= playerCar.y + 4) {
            if (obs.x >= playerCar.x - 3 && obs.x <= playerCar.x + 3) {
                playCrashSound();
                raceOver = true;
                playerCar.crashed = true;
                if (raceScore > raceHighScore) {
                    raceHighScore = raceScore;
                }
                return;
            }
        }
    }

    if (gameDistance % 50 == 0) {
        spawnObstacle();
    }

    if (playerCar.speed > 0) {
        playEngineSound();
    }
}

void drawRoad() {
    static int lastGameDistance = -1;
    if (gameDistance == lastGameDistance) return;
    lastGameDistance = gameDistance;

    for (int y = 5; y < 35; y++) {
        for (int x = roadPosition - 2; x < roadPosition + roadWidth + 2; x++) {
            if (x == roadPosition - 2 || x == roadPosition + roadWidth + 1) {
                putChar(x, y, '█', DARK_GRAY, BLACK);
            }
            else if ((y + gameDistance / 2) % 2 == 0 && x == roadPosition + roadWidth / 2) {
                putChar(x, y, '|', YELLOW, BLACK);
            }
            else if (x >= roadPosition && x < roadPosition + roadWidth) {
                if ((x - roadPosition) % 7 == 3) {
                    putChar(x, y, '.', LIGHT_GRAY, BLACK);
                }
                else {
                    putChar(x, y, ' ', LIGHT_GRAY, BLACK);
                }
            }
        }
    }
}

CompileResult compileCppCode(const string& code) {
    CompileResult result;
    result.success = false;
    result.errors = 0;
    result.warnings = 0;
    result.output = "";

    ofstream tempFile("temp_sosioc.cpp");
    if (!tempFile.is_open()) {
        result.output = "ERROR: Cannot create temporary file";
        return result;
    }

    tempFile << code;
    tempFile.close();

    result.output = "Compilation in progress...\n";

    string compileCmd = "g++ temp_sosioc.cpp -o temp_sosioc.exe 2>&1";

    FILE* pipe = _popen(compileCmd.c_str(), "r");
    if (!pipe) {
        result.output = "ERROR: Cannot start compiler";
        return result;
    }

    char buffer[128];
    while (fgets(buffer, sizeof(buffer), pipe) != NULL) {
        result.output += buffer;
        if (strstr(buffer, "error")) result.errors++;
        if (strstr(buffer, "warning")) result.warnings++;
    }

    int status = _pclose(pipe);

    if (status == 0) {
        result.success = true;
        result.output += "\nCompilation successful!";
    }
    else {
        result.output += "\nCompilation failed!";
    }

    remove("temp_sosioc.cpp");

    return result;
}

void runCompiledProgram() {
    if (!lastCompile.success) {
        printStatusBar("No successful compilation to run", LIGHT_RED);
        return;
    }

    printStatusBar("Running program...", LIGHT_CYAN);

    system("temp_sosioc.exe");

    remove("temp_sosioc.exe");

    printStatusBar("Program execution completed", LIGHT_GREEN);
}

void saveCppCode(const string& filename) {
    string fullPath = "C:\\SOSIOC\\main\\cpp\\" + filename;
    if (filename.find(".cpp") == string::npos) {
        fullPath += ".cpp";
    }

    ofstream file(fullPath);
    if (file.is_open()) {
        file << cppSourceCode;
        file.close();
        printStatusBar("Code saved to: " + fullPath, LIGHT_GREEN);
    }
    else {
        printStatusBar("ERROR: Cannot save file", LIGHT_RED);
    }
}

void loadCppCode(const string& filename) {
    string fullPath = "C:\\SOSIOC\\main\\cpp\\" + filename;
    if (filename.find(".cpp") == string::npos) {
        fullPath += ".cpp";
    }

    ifstream file(fullPath);
    if (file.is_open()) {
        cppSourceCode = "";
        string line;
        while (getline(file, line)) {
            cppSourceCode += line + "\n";
        }
        file.close();
        printStatusBar("Code loaded from: " + fullPath, LIGHT_GREEN);
    }
    else {
        printStatusBar("ERROR: Cannot load file", LIGHT_RED);
    }
}

void executeExternalCommand(const string& cmd) {
    system(cmd.c_str());
}

string getFileType(const string& path, bool& isText, bool& isDrawing, bool& isMusic) {
    DWORD attr = GetFileAttributesA(path.c_str());
    isText = false;
    isDrawing = false;
    isMusic = false;

    if (attr == INVALID_FILE_ATTRIBUTES) {
        size_t dotPos = path.find_last_of('.');
        if (dotPos != string::npos) {
            string ext = path.substr(dotPos);
            transform(ext.begin(), ext.end(), ext.begin(), ::tolower);
            if (ext == ".txt") {
                isText = true;
                return "Text";
            }
            if (ext == ".draw") {
                isDrawing = true;
                return "Drawing";
            }
            if (ext == ".music") {
                isMusic = true;
                return "Music";
            }
            if (ext == ".cpp" || ext == ".c" || ext == ".h") {
                isText = true;
                return "C++ Code";
            }
        }
        return "Unknown";
    }

    if (attr & FILE_ATTRIBUTE_DIRECTORY) return "Folder";

    size_t dotPos = path.find_last_of('.');
    if (dotPos != string::npos) {
        string ext = path.substr(dotPos);
        transform(ext.begin(), ext.end(), ext.begin(), ::tolower);
        if (ext == ".txt" || ext == ".cpp" || ext == ".h" || ext == ".hpp" || ext == ".c") {
            isText = true;
            if (ext == ".txt") return "Text";
            if (ext == ".cpp" || ext == ".c") return "C++ Code";
            return "Header";
        }
        if (ext == ".draw") {
            isDrawing = true;
            return "Drawing";
        }
        if (ext == ".music") {
            isMusic = true;
            return "Music";
        }
        if (ext == ".exe" || ext == ".dll" || ext == ".bat" || ext == ".com") return "Executable";
        if (ext == ".jpg" || ext == ".png" || ext == ".bmp" || ext == ".gif") return "Image";
    }
    return "File";
}

void loadFileList() {
    fileList.clear();
    char buffer[MAX_PATH];
    GetCurrentDirectoryA(MAX_PATH, buffer);
    printStatusBar("Loading files from: " + string(buffer), LIGHT_CYAN);
    FileInfo parent;
    parent.name = "..";
    parent.type = "Parent";
    parent.size = 0;
    parent.modified = "";
    parent.isDirectory = true;
    parent.isTextFile = false;
    parent.isDrawingFile = false;
    parent.isMusicFile = false;
    fileList.push_back(parent);
    WIN32_FIND_DATAA findData;
    HANDLE hFind;
    string searchPath = currentDir + "\\*.*";
    hFind = FindFirstFileA(searchPath.c_str(), &findData);
    if (hFind == INVALID_HANDLE_VALUE) {
        return;
    }
    do {
        string filename = findData.cFileName;
        if (filename == "." || filename == "..") continue;
        if (!showHiddenFiles && (findData.dwFileAttributes & FILE_ATTRIBUTE_HIDDEN)) continue;
        FileInfo fi;
        fi.name = filename;
        bool isText = false;
        bool isDrawing = false;
        bool isMusic = false;
        fi.type = getFileType(filename, isText, isDrawing, isMusic);
        fi.isTextFile = isText;
        fi.isDrawingFile = isDrawing;
        fi.isMusicFile = isMusic;
        fi.isDirectory = (findData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) != 0;
        if (!fi.isDirectory) {
            fi.size = ((long long)findData.nFileSizeHigh << 32) | findData.nFileSizeLow;
        }
        else {
            fi.size = 0;
        }
        FILETIME ft = findData.ftLastWriteTime;
        SYSTEMTIME st;
        FileTimeToSystemTime(&ft, &st);
        char timeStr[20];
        sprintf(timeStr, "%02d.%02d.%04d %02d:%02d",
            st.wDay, st.wMonth, st.wYear, st.wHour, st.wMinute);
        fi.modified = timeStr;
        fileList.push_back(fi);
    } while (FindNextFileA(hFind, &findData) != 0);
    FindClose(hFind);
}

void loadProcesses() {
    processes.clear();
    HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (hSnapshot == INVALID_HANDLE_VALUE) return;
    PROCESSENTRY32 pe32;
    pe32.dwSize = sizeof(PROCESSENTRY32);
    if (Process32First(hSnapshot, &pe32)) {
        do {
            ProcessInfo pi;
            pi.pid = pe32.th32ProcessID;
            char name[260];
            WideCharToMultiByte(CP_ACP, 0, pe32.szExeFile, -1, name, 260, NULL, NULL);
            pi.name = name;
            HANDLE hProcess = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, pe32.th32ProcessID);
            if (hProcess) {
                PROCESS_MEMORY_COUNTERS pmc;
                if (GetProcessMemoryInfo(hProcess, &pmc, sizeof(pmc))) {
                    pi.memoryUsage = (DWORD)(pmc.WorkingSetSize / 1024);
                }
                CloseHandle(hProcess);
            }
            processes.push_back(pi);
        } while (Process32Next(hSnapshot, &pe32) && processes.size() < 100);
    }
    CloseHandle(hSnapshot);
}

void drawDesktop() {
    static int lastSelectedApp = -1;
    static int lastAppScrollOffset = -1;

    bool selectionChanged = (lastSelectedApp != selectedApp);
    bool scrollChanged = (lastAppScrollOffset != appScrollOffset);

    if (!needRedraw && !selectionChanged && !scrollChanged) {
        drawSystemPanel();
        drawTaskBar();
        return;
    }

    needRedraw = false;
    lastSelectedApp = selectedApp;
    lastAppScrollOffset = appScrollOffset;

    int centerX = CONSOLE_WIDTH / 2 - 50;

    drawBox(centerX, 5, 60, 18, BLUE, "SOSIOC Desktop", true);

    putString(centerX + 5, 8, "Welcome to SOSIOC Operating System", YELLOW, BLACK);
    putString(centerX + 5, 9, "Virtual Disk: C:\\SOSIOC\\main", LIGHT_CYAN, BLACK);
    putString(centerX + 5, 10, "Available Applications:", LIGHT_CYAN, BLACK);

    int visibleApps = 4;
    int startIdx = appScrollOffset;
    int endIdx = min(startIdx + visibleApps, (int)applications.size());

    for (int i = startIdx; i < endIdx; i++) {
        int displayIdx = i - startIdx;
        if (i == selectedApp) {
            putChar(centerX + 7, 12 + displayIdx * 2, char(16), LIGHT_GREEN, BLACK);
            putString(centerX + 9, 12 + displayIdx * 2, to_string(i + 1) + ". " + applications[i].name, YELLOW, BLACK);
            putString(centerX + 15, 13 + displayIdx * 2, applications[i].description, LIGHT_GRAY, BLACK);
        }
        else {
            putChar(centerX + 7, 12 + displayIdx * 2, ' ', LIGHT_GRAY, BLACK);
            putString(centerX + 9, 12 + displayIdx * 2, to_string(i + 1) + ". " + applications[i].name, LIGHT_GRAY, BLACK);
        }
    }

    if (applications.size() > (size_t)visibleApps) {
        if (appScrollOffset > 0) {
            putChar(centerX + 50, 12, '^', LIGHT_GREEN, BLACK);
        }
        else {
            putChar(centerX + 50, 12, '^', DARK_GRAY, BLACK);
        }

        if (appScrollOffset + visibleApps < (int)applications.size()) {
            putChar(centerX + 50, 12 + visibleApps - 1, 'v', LIGHT_GREEN, BLACK);
        }
        else {
            putChar(centerX + 50, 12 + visibleApps - 1, 'v', DARK_GRAY, BLACK);
        }

        string scrollInfo = to_string(appScrollOffset + 1) + "-" +
            to_string(min(appScrollOffset + visibleApps, (int)applications.size())) +
            "/" + to_string(applications.size());
        putString(centerX + 50, 12 + visibleApps / 2, scrollInfo, LIGHT_GRAY, BLACK);
    }

    drawBox(centerX, 20, 60, 4, GREEN, "Quick Actions", true);
    putString(centerX + 2, 21, "Ctrl+H: Help", LIGHT_GREEN, BLACK);
    putString(centerX + 2, 22, "Ctrl+P: Toggle Panel", LIGHT_GREEN, BLACK);
    putString(centerX + 35, 21, "ESC: Shutdown", LIGHT_RED, BLACK);
    putString(centerX + 35, 22, "ENTER: Launch App", LIGHT_CYAN, BLACK);

    drawSystemPanel();
    drawTaskBar();
    printStatusBar("Use arrow keys to select application, ENTER to launch. Scroll with PGUP/PGDN", LIGHT_GREEN);
}


bool handleDesktopMode() {
    clearScreen();
    updateScreen();
    needRedraw = true;

    selectedApp = min(selectedApp, (int)applications.size() - 1);
    appScrollOffset = min(appScrollOffset, max(0, (int)applications.size() - 4));
    needRedraw = true;

    while (true) {
        getSystemInfo();

        if (clock() - lastBlink > 500) {
            blinkState = !blinkState;
            lastBlink = clock();
            needRedraw = true;
        }

        drawDesktop();
        updateScreen();

        if (_kbhit()) {
            int key = _getch();
            if (key == 0 || key == 224) {
                key = _getch();
                if (key == 72) {
                    playMoveSound();
                    if (selectedApp > 0) {
                        selectedApp--;
                        if (selectedApp < appScrollOffset) {
                            appScrollOffset = selectedApp;
                        }
                        needRedraw = true;
                    }
                }
                if (key == 80) {
                    playMoveSound();
                    if (selectedApp < (int)applications.size() - 1) {
                        selectedApp++;
                        if (selectedApp >= appScrollOffset + 4) {
                            appScrollOffset = selectedApp - 3;
                        }
                        needRedraw = true;
                    }
                }
                if (key == 73) {
                    playMoveSound();
                    selectedApp = max(0, selectedApp - 4);
                    appScrollOffset = max(0, appScrollOffset - 4);
                    needRedraw = true;
                }
                if (key == 81) {
                    playMoveSound();
                    selectedApp = min((int)applications.size() - 1, selectedApp + 4);
                    appScrollOffset = min(max(0, (int)applications.size() - 4), appScrollOffset + 4);
                    needRedraw = true;
                }
            }
            else if (key == 13) {
                playSelectSound();
                playOpenSound();
                switch (selectedApp) {
                case 0: currentScreen = CALCULATOR; return true;
                case 1: currentScreen = FILE_MANAGER; return true;
                case 2: currentScreen = TEXT_EDITOR; return true;
                case 3: currentScreen = SETTINGS; return true;
                case 4: currentScreen = TASK_MANAGER; return true;
                case 5: currentScreen = MEMORY_GAME; return true;
                case 6: currentScreen = PROCESS_MONITOR; return true;
                case 7: currentScreen = PAINT; return true;
                case 8: currentScreen = MUSIC_PLAYER; return true;
                case 9: currentScreen = WEATHER; return true;
                case 10: currentScreen = ARCADE_GAME; return true;
                case 11: currentScreen = CLOCK; return true;
                case 12: currentScreen = NEW_YEAR_QUEST; return true;
                case 13: currentScreen = MUSIC_EDITOR; return true;
                case 14: currentScreen = RACING_GAME; return true;
                case 15: currentScreen = CPLUS_COMPILER; return true;
                }
            }
            else if (key == 27) {
                playCloseSound();
                return false;
            }
            else if ((key == 8) && (GetKeyState(VK_CONTROL) & 0x8000)) {
                playSelectSound();
                currentScreen = HELP;
                return true;
            }
            else if ((key == 16) && (GetKeyState(VK_CONTROL) & 0x8000)) {
                playSelectSound();
                systemMonitorVisible = !systemMonitorVisible;
                needRedraw = true;
            }
            else if (key >= '1' && key <= '9') {
                playSelectSound();
                int appNum = key - '1';
                if (appNum < (int)applications.size()) {
                    selectedApp = appNum;
                    if (selectedApp < appScrollOffset) {
                        appScrollOffset = selectedApp;
                    }
                    else if (selectedApp >= appScrollOffset + 4) {
                        appScrollOffset = selectedApp - 3;
                    }
                    playOpenSound();
                    switch (selectedApp) {
                    case 0: currentScreen = CALCULATOR; return true;
                    case 1: currentScreen = FILE_MANAGER; return true;
                    case 2: currentScreen = TEXT_EDITOR; return true;
                    case 3: currentScreen = SETTINGS; return true;
                    case 4: currentScreen = TASK_MANAGER; return true;
                    case 5: currentScreen = MEMORY_GAME; return true;
                    case 6: currentScreen = PROCESS_MONITOR; return true;
                    case 7: currentScreen = PAINT; return true;
                    case 8: currentScreen = MUSIC_PLAYER; return true;
                    case 9: currentScreen = WEATHER; return true;
                    case 10: currentScreen = ARCADE_GAME; return true;
                    case 11: currentScreen = CLOCK; return true;
                    case 12: currentScreen = NEW_YEAR_QUEST; return true;
                    case 13: currentScreen = MUSIC_EDITOR; return true;
                    case 14: currentScreen = RACING_GAME; return true;
                    case 15: currentScreen = CPLUS_COMPILER; return true;
                    }
                }
            }
        }
        Sleep(50);
    }
    return false;
}

int getPrecedence(char op) {
    if (op == '+' || op == '-') return 1;
    if (op == '*' || op == '/' || op == '%') return 2;
    if (op == '^') return 3;
    return 0;
}

double applyOperation(double a, double b, char op) {
    switch (op) {
    case '+': return a + b;
    case '-': return a - b;
    case '*': return a * b;
    case '/':
        if (b == 0) throw runtime_error("Division by zero");
        return a / b;
    case '%':
        if ((int)b == 0) throw runtime_error("Modulo by zero");
        return fmod(a, b);
    case '^': return pow(a, b);
    default: return 0;
    }
}

string replaceVariables(const string& expr) {
    string result = expr;
    size_t pos = 0;
    while ((pos = result.find("ANS", pos)) != string::npos) {
        if ((pos == 0 || !isalnum(result[pos - 1])) &&
            (pos + 3 >= result.length() || !isalnum(result[pos + 3]))) {
            double ansValue = history.empty() ? 0 : history.back().result;
            string ansStr = to_string(ansValue);
            result.replace(pos, 3, ansStr);
            pos += ansStr.length();
        }
        else {
            pos += 3;
        }
    }
    pos = 0;
    while ((pos = result.find("PI", pos)) != string::npos) {
        if ((pos == 0 || !isalnum(result[pos - 1])) &&
            (pos + 2 >= result.length() || !isalnum(result[pos + 2]))) {
            result.replace(pos, 2, "3.1415926535");
            pos += 12;
        }
        else {
            pos += 2;
        }
    }
    pos = 0;
    while ((pos = result.find("E", pos)) != string::npos) {
        bool isVariable = true;
        if (pos > 0) {
            char prev = result[pos - 1];
            if (isalnum(prev) || prev == '.' || prev == ')') {
                isVariable = false;
            }
        }
        if (pos + 1 < result.length()) {
            char next = result[pos + 1];
            if (isalnum(next) || next == '(') {
                isVariable = false;
            }
        }
        if (isVariable) {
            result.replace(pos, 1, "2.7182818284");
            pos += 12;
        }
        else {
            pos++;
        }
    }
    pos = 0;
    while ((pos = result.find("ans", pos)) != string::npos) {
        if ((pos == 0 || !isalnum(result[pos - 1])) &&
            (pos + 3 >= result.length() || !isalnum(result[pos + 3]))) {
            double ansValue = history.empty() ? 0 : history.back().result;
            string ansStr = to_string(ansValue);
            result.replace(pos, 3, ansStr);
            pos += ansStr.length();
        }
        else {
            pos += 3;
        }
    }
    pos = 0;
    while ((pos = result.find("pi", pos)) != string::npos) {
        if ((pos == 0 || !isalnum(result[pos - 1])) &&
            (pos + 2 >= result.length() || !isalnum(result[pos + 2]))) {
            result.replace(pos, 2, "3.1415926535");
            pos += 12;
        }
        else {
            pos += 2;
        }
    }
    pos = 0;
    while ((pos = result.find("e", pos)) != string::npos) {
        bool isVariable = true;
        if (pos > 0) {
            char prev = result[pos - 1];
            if (isalnum(prev) || prev == '.' || prev == ')') {
                isVariable = false;
            }
        }
        if (pos + 1 < result.length()) {
            char next = result[pos + 1];
            if (isalnum(next) || next == '(') {
                isVariable = false;
            }
        }
        if (isVariable) {
            result.replace(pos, 1, "2.7182818284");
            pos += 12;
        }
        else {
            pos++;
        }
    }
    return result;
}

string replaceFunctions(const string& expr) {
    string result = expr;
    map<string, string> functions = {
        {"sin(", "S("},
        {"cos(", "C("},
        {"tan(", "T("},
        {"sqrt(", "Q("},
        {"log(", "L("},
        {"exp(", "E("},
        {"abs(", "A("}
    };
    for (const auto& func : functions) {
        size_t pos = 0;
        while ((pos = result.find(func.first, pos)) != string::npos) {
            result.replace(pos, func.first.length(), func.second);
            pos += func.second.length();
        }
    }
    return result;
}

double evaluateFunction(char func, double value) {
    switch (func) {
    case 'S': return sin(value * 3.1415926535 / 180.0);
    case 'C': return cos(value * 3.1415926535 / 180.0);
    case 'T': return tan(value * 3.1415926535 / 180.0);
    case 'Q':
        if (value < 0) throw runtime_error("Square root of negative number");
        return sqrt(value);
    case 'L':
        if (value <= 0) throw runtime_error("Logarithm of non-positive number");
        return log(value);
    case 'E': return exp(value);
    case 'A': return abs(value);
    default: return value;
    }
}

double evaluateExpression(string expr) {
    expr = replaceVariables(expr);
    expr = replaceFunctions(expr);
    stack<double> values;
    stack<char> ops;
    for (size_t i = 0; i < expr.length(); i++) {
        if (expr[i] == ' ') continue;
        if (expr[i] == 'S' || expr[i] == 'C' || expr[i] == 'T' ||
            expr[i] == 'Q' || expr[i] == 'L' || expr[i] == 'E' || expr[i] == 'A') {
            char func = expr[i];
            i++;
            if (expr[i] != '(') throw runtime_error("Function missing '('");
            i++;
            string argStr;
            int parenCount = 1;
            while (i < expr.length() && parenCount > 0) {
                if (expr[i] == '(') parenCount++;
                if (expr[i] == ')') parenCount--;
                if (parenCount == 0) break;
                argStr += expr[i];
                i++;
            }
            if (parenCount != 0) throw runtime_error("Function missing ')'");
            double arg = evaluateExpression(argStr);
            double result = evaluateFunction(func, arg);
            values.push(result);
        }
        else if (expr[i] == '(') {
            ops.push(expr[i]);
        }
        else if (isdigit(expr[i]) || expr[i] == '.') {
            string numStr;
            while (i < expr.length() && (isdigit(expr[i]) || expr[i] == '.')) {
                numStr += expr[i];
                i++;
            }
            i--;
            values.push(stod(numStr));
        }
        else if (expr[i] == ')') {
            while (!ops.empty() && ops.top() != '(') {
                double val2 = values.top(); values.pop();
                double val1 = values.top(); values.pop();
                char op = ops.top(); ops.pop();
                values.push(applyOperation(val1, val2, op));
            }
            if (!ops.empty()) ops.pop();
        }
        else {
            while (!ops.empty() && getPrecedence(ops.top()) >= getPrecedence(expr[i])) {
                double val2 = values.top(); values.pop();
                double val1 = values.top(); values.pop();
                char op = ops.top(); ops.pop();
                values.push(applyOperation(val1, val2, op));
            }
            ops.push(expr[i]);
        }
    }
    while (!ops.empty()) {
        double val2 = values.top(); values.pop();
        double val1 = values.top(); values.pop();
        char op = ops.top(); ops.pop();
        values.push(applyOperation(val1, val2, op));
    }
    return values.top();
}

bool isValidComplexExpression(string expr) {
    expr = replaceVariables(expr);
    int parenCount = 0;
    for (char c : expr) {
        if (c == '(') parenCount++;
        if (c == ')') parenCount--;
        if (parenCount < 0) return false;
    }
    return parenCount == 0;
}

string toHex(int n) {
    if (n == 0) return "0x0";
    string hex = "";
    char hexChars[] = "0123456789ABCDEF";
    unsigned int un = n;
    if (n < 0) un = (unsigned int)n;
    while (un > 0) {
        hex = hexChars[un % 16] + hex;
        un /= 16;
    }
    if (n < 0) {
        return "-0x" + hex;
    }
    return "0x" + hex;
}

string toBinary(int n) {
    if (n == 0) return "0b0";
    string bin = "";
    unsigned int un = n;
    if (n < 0) un = (unsigned int)n;
    for (int i = 31; i >= 0; i--) {
        bin += (un & (1 << i)) ? '1' : '0';
    }
    while (bin.length() > 1 && bin[0] == '0') {
        bin.erase(0, 1);
    }
    if (n < 0) {
        return "-0b" + bin;
    }
    return "0b" + bin;
}

void addToHistory(const string& expr, double result) {
    if (history.size() >= HISTORY_LIMIT) {
        history.erase(history.begin());
    }
    Calculation calc;
    calc.expression = expr;
    calc.result = result;
    time_t now = time(0);
    tm* ltm = localtime(&now);
    char timeStr[9];
    strftime(timeStr, sizeof(timeStr), "%H:%M:%S", ltm);
    calc.time = timeStr;
    calc.hex_result = toHex((int)result);
    calc.bin_result = toBinary((int)result);
    history.push_back(calc);
}

void printCalculator() {
    static string lastExpression = "";
    static int lastCursorPos = -1;
    static bool lastBlinkState = false;

    bool exprChanged = (currentExpression != lastExpression);
    bool cursorChanged = (cursorPos != lastCursorPos);
    bool blinkChanged = (blinkState != lastBlinkState);

    if (!needRedraw && !exprChanged && !cursorChanged && !blinkChanged) {
        return;
    }

    lastExpression = currentExpression;
    lastCursorPos = cursorPos;
    lastBlinkState = blinkState;

    drawBox(2, 4, CONSOLE_WIDTH - 40, 25, GREEN, "Calculator", true);

    putString(4, 6, "Enter expression:", YELLOW, BLACK);
    putString(4 + 18, 6, " (PI=3.14159, E=2.71828, ANS=last result)", LIGHT_GRAY, BLACK);

    drawBox(4, 7, CONSOLE_WIDTH - 44, 3, CYAN);

    string displayExpr = currentExpression;
    if (showCursor && blinkState) {
        if (cursorPos <= (int)displayExpr.length()) {
            displayExpr.insert(cursorPos, "_");
        }
    }

    putString(6, 8, "> " + displayExpr, LIGHT_GRAY, CYAN);

    for (int i = displayExpr.length() + 2; i < CONSOLE_WIDTH - 50; i++) {
        putChar(6 + i, 8, ' ', LIGHT_GRAY, CYAN);
    }

    drawBox(4, 11, CONSOLE_WIDTH - 80, 10, BLUE, "Functions", true);

    string functions[] = {
        "sin(x) - sine",
        "cos(x) - cosine",
        "tan(x) - tangent",
        "sqrt(x) - square root",
        "log(x) - natural log",
        "exp(x) - exponent",
        "abs(x) - absolute value"
    };

    for (int i = 0; i < 7; i++) {
        putChar(6, 13 + i, char(26), LIGHT_GRAY, BLACK);
        putString(8, 13 + i, functions[i], LIGHT_GRAY, BLACK);
    }

    drawBox(30, 11, 50, 10, MAGENTA, "Last Results", true);

    int startIdx = max(0, (int)history.size() - 8);
    for (int i = startIdx; i < (int)history.size(); i++) {
        string display = history[i].expression;
        if (display.length() > 15) display = display.substr(0, 12) + "...";
        string line = to_string(i + 1) + ". " + display + " = " + to_string(history[i].result).substr(0, 10);
        putString(32, 13 + (i - startIdx), line, LIGHT_GRAY, BLACK);
        putString(32 + display.length() + 6, 13 + (i - startIdx), to_string(history[i].result).substr(0, 10), LIGHT_GREEN, BLACK);
    }

    drawBox(4, 22, CONSOLE_WIDTH - 44, 5, BROWN, "Memory Variables", true);
    putString(6, 24, "PI = 3.1415926535", LIGHT_CYAN, BLACK);
    putString(30, 24, "E = 2.7182818284", LIGHT_CYAN, BLACK);
    putString(54, 24, "ANS = ", LIGHT_CYAN, BLACK);

    if (!history.empty()) {
        putString(60, 24, to_string(history.back().result), LIGHT_GRAY, BLACK);
    }
    else {
        putString(60, 24, "0", LIGHT_GRAY, BLACK);
    }

    putString(4, 28, "Keys: ", YELLOW, BLACK);
    putString(4 + 6, 28, "ENTER=Calculate  ESC=Back  BACKSPACE=Delete", LIGHT_GRAY, BLACK);
}

bool handleCalculatorMode() {
    clearScreen();
    updateScreen();
    needRedraw = true;
    currentExpression = "";
    cursorPos = 0;
    needRedraw = true;

    while (true) {
        getSystemInfo();

        if (clock() - lastBlink > 500) {
            blinkState = !blinkState;
            lastBlink = clock();
        }

        printCalculator();
        updateScreen();

        if (_kbhit()) {
            int key = _getch();
            if (key == 0 || key == 224) {
                key = _getch();
                if (key == 75 && cursorPos > 0) {
                    playMoveSound();
                    cursorPos--;
                }
                if (key == 77 && cursorPos < (int)currentExpression.length()) {
                    playMoveSound();
                    cursorPos++;
                }
                if (key == 83 && cursorPos < (int)currentExpression.length()) {
                    playMoveSound();
                    currentExpression.erase(cursorPos, 1);
                }
            }
            else if (key == 13) {
                playSelectSound();
                if (currentExpression.empty()) {
                    playErrorSound();
                    printStatusBar("ERROR: Empty expression", LIGHT_RED);
                    continue;
                }
                try {
                    string expr = currentExpression;
                    if (!isValidComplexExpression(expr)) {
                        playErrorSound();
                        printStatusBar("ERROR: Invalid parentheses", LIGHT_RED);
                        continue;
                    }
                    double result = evaluateExpression(expr);
                    addToHistory(currentExpression, result);

                    drawBox(30, 15, 60, 5, GREEN, "Result", true);
                    putString(32, 17, "Decimal: ", YELLOW, BLACK);
                    putString(32 + 9, 17, to_string(result).substr(0, 15), LIGHT_GREEN, BLACK);
                    putString(32, 18, "Hex:     ", YELLOW, BLACK);
                    putString(32 + 9, 18, toHex((int)result), LIGHT_CYAN, BLACK);
                    putString(32, 19, "Binary:  ", YELLOW, BLACK);
                    putString(32 + 9, 19, toBinary((int)result).substr(0, 25), LIGHT_MAGENTA, BLACK);

                    playSuccessSound();
                    printStatusBar("Calculation successful", LIGHT_GREEN);
                    currentExpression = "";
                    cursorPos = 0;
                    needRedraw = true;
                    updateScreen();
                    _getch();
                }
                catch (const exception& e) {
                    playErrorSound();
                    printStatusBar("ERROR: " + string(e.what()), LIGHT_RED);
                    _getch();
                }
            }
            else if (key == 8 && cursorPos > 0) {
                playMoveSound();
                cursorPos--;
                currentExpression.erase(cursorPos, 1);
            }
            else if (key == 27) {
                playCloseSound();
                currentScreen = DESKTOP;
                return true;
            }
            else if ((key == 8) && (GetKeyState(VK_CONTROL) & 0x8000)) {
                playSelectSound();
                currentScreen = HELP;
                return true;
            }
            else if (isprint(key)) {
                playMoveSound();
                currentExpression.insert(cursorPos, 1, char(key));
                cursorPos++;
            }
        }
        Sleep(50);
    }
    return false;
}


string formatSize(long long bytes) {
    const char* sizes[] = { "B", "KB", "MB", "GB" };
    int i = 0;
    double dblBytes = bytes;
    while (dblBytes >= 1024 && i < 3) {
        dblBytes /= 1024;
        i++;
    }
    stringstream ss;
    ss << fixed << setprecision(1) << dblBytes << " " << sizes[i];
    return ss.str();
}

void addToCommandHistory(const string& cmd) {
    if (cmd.empty()) return;
    if (commandHistory.size() >= COMMAND_HISTORY_LIMIT) {
        commandHistory.erase(commandHistory.begin());
    }
    commandHistory.push_back(cmd);
    commandHistoryIndex = (int)commandHistory.size();
}

vector<string> splitCommand(const string& cmd) {
    vector<string> parts;
    stringstream ss(cmd);
    string part;
    while (ss >> part) {
        parts.push_back(part);
    }
    return parts;
}

bool createFile(const string& name) {
    char buffer[MAX_PATH];
    GetCurrentDirectoryA(MAX_PATH, buffer);
    string currentPath(buffer);

    string fullPath;
    if (currentDir == ".") {
        fullPath = currentPath + "\\" + name;
    }
    else {
        fullPath = currentDir + "\\" + name;
    }

    ofstream file(fullPath);
    if (!file.is_open()) {
        printStatusBar("ERROR: Cannot create file at: " + fullPath, LIGHT_RED);
        return false;
    }

    if (name.find(".draw") != string::npos) {
        file << "PAINTFILE\n";
        file << "Version: 1.0\n";
        file << "0 0 1\n";
        file << "79 29 4\n";
        file << "ENDFILE\n";
    }
    else if (name.find(".music") != string::npos) {
        file << "SOSIOC_MUSIC_v1.0\n";
        file << "Name: New Composition\n";
        file << "Notes:\n";
        file << "// Example notes\n";
        file << "note(262, 500, 100)\n";
        file << "note(294, 500, 100)\n";
        file << "note(330, 500, 100)\n";
    }
    else if (name.find(".cpp") != string::npos) {
        file << "#include <iostream>\n";
        file << "using namespace std;\n\n";
        file << "int main() {\n";
        file << "    cout << \"Hello from SOSIOC!\" << endl;\n";
        file << "    return 0;\n";
        file << "}\n";
    }
    else if (name.find(".txt") != string::npos || name.find(".") == string::npos) {
        file << "File created by SOSIOC File Manager\n";
        file << "Created: " << __DATE__ << " " << __TIME__ << "\n";
        file << "This is a sample text file.\n";
        file << "You can edit it in the Text Editor.\n";
    }

    file.close();

    if (GetFileAttributesA(fullPath.c_str()) == INVALID_FILE_ATTRIBUTES) {
        printStatusBar("ERROR: File not created: " + fullPath, LIGHT_RED);
        return false;
    }

    printStatusBar("File created: " + fullPath, LIGHT_GREEN);
    loadFileList();

    return true;
}

bool createFolder(const string& name) {
    string fullPath;
    if (currentDir == ".") {
        fullPath = name;
    }
    else {
        fullPath = currentDir + "\\" + name;
    }
    if (_mkdir(fullPath.c_str()) != 0) {
        return false;
    }
    return true;
}

bool renameFileOrFolder(const string& oldName, const string& newName) {
    string oldPath, newPath;
    if (currentDir == ".") {
        oldPath = oldName;
        newPath = newName;
    }
    else {
        oldPath = currentDir + "\\" + oldName;
        newPath = currentDir + "\\" + newName;
    }
    if (rename(oldPath.c_str(), newPath.c_str()) != 0) {
        return false;
    }
    return true;
}

bool deleteFileOrFolder(const string& name) {
    string fullPath;
    if (currentDir == ".") {
        fullPath = name;
    }
    else {
        fullPath = currentDir + "\\" + name;
    }
    DWORD attr = GetFileAttributesA(fullPath.c_str());
    if (attr == INVALID_FILE_ATTRIBUTES) {
        return false;
    }
    if (attr & FILE_ATTRIBUTE_DIRECTORY) {
        if (RemoveDirectoryA(fullPath.c_str()) == 0) {
            return false;
        }
    }
    else {
        if (DeleteFileA(fullPath.c_str()) == 0) {
            return false;
        }
    }
    return true;
}

void changeDirectory(const string& dir) {
    if (dir == "..") {
        char buffer[MAX_PATH];
        GetCurrentDirectoryA(MAX_PATH, buffer);
        string currentPath(buffer);
        size_t lastSlash = currentPath.find_last_of("\\/");
        if (lastSlash != string::npos) {
            currentPath = currentPath.substr(0, lastSlash);
            if (SetCurrentDirectoryA(currentPath.c_str()) != 0) {
                currentDir = currentPath;
            }
        }
    }
    else if (dir == ".") {
        char buffer[MAX_PATH];
        GetCurrentDirectoryA(MAX_PATH, buffer);
        currentDir = buffer;
    }
    else {
        string newPath;
        char buffer[MAX_PATH];
        GetCurrentDirectoryA(MAX_PATH, buffer);
        string currentPath(buffer);

        if (SetCurrentDirectoryA(dir.c_str()) != 0) {
            GetCurrentDirectoryA(MAX_PATH, buffer);
            currentDir = buffer;
        }
        else {
            SetCurrentDirectoryA(currentPath.c_str());
            newPath = currentPath + "\\" + dir;
            if (SetCurrentDirectoryA(newPath.c_str()) != 0) {
                GetCurrentDirectoryA(MAX_PATH, buffer);
                currentDir = buffer;
            }
            else {
                SetCurrentDirectoryA(currentPath.c_str());
                playErrorSound();
                printStatusBar("ERROR: Directory not found: " + dir, LIGHT_RED);
            }
        }
    }

    char buffer[MAX_PATH];
    GetCurrentDirectoryA(MAX_PATH, buffer);
    currentDir = buffer;
}

void listAllFiles() {
    WIN32_FIND_DATAA findData;
    HANDLE hFind;
    string searchPath = currentDir + "\\*.*";
    hFind = FindFirstFileA(searchPath.c_str(), &findData);
    if (hFind == INVALID_HANDLE_VALUE) {
        return;
    }
    do {
        string filename = findData.cFileName;
        if (filename == "." || filename == "..") continue;
        if (!showHiddenFiles && (findData.dwFileAttributes & FILE_ATTRIBUTE_HIDDEN)) continue;
        string type = (findData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) ? "Folder" : "File";
        putString(6, 22, filename + " - " + type, LIGHT_GRAY, BLACK);
    } while (FindNextFileA(hFind, &findData) != 0);
    FindClose(hFind);
}

void showCommandHelp() {
    drawBox(20, 12, 80, 14, CYAN, "Command Help", true);

    putString(22, 14, "Available commands:", YELLOW, BLACK);
    putString(22, 16, "--add [filename]              Create file (specify extension)", LIGHT_GRAY, BLACK);
    putString(22, 17, "                               Example: --add picture.draw", LIGHT_GRAY, BLACK);
    putString(22, 18, "                               Example: --add song.music", LIGHT_GRAY, BLACK);
    putString(22, 19, "                               Example: --add note.txt", LIGHT_GRAY, BLACK);
    putString(22, 20, "--addf [foldername]           Create folder in current directory", LIGHT_GRAY, BLACK);
    putString(22, 21, "--rename [old] [new]          Rename file or folder", LIGHT_GRAY, BLACK);
    putString(22, 22, "--delete [name]               Delete file or folder", LIGHT_GRAY, BLACK);
    putString(22, 23, "--cd [directory]              Change current directory", LIGHT_GRAY, BLACK);
    putString(22, 24, "--list                        List all files in current directory", LIGHT_GRAY, BLACK);
    putString(22, 25, "--clear                       Clear command console", LIGHT_GRAY, BLACK);
    putString(22, 26, "--clearf [folder]             Clear ALL files in folder", LIGHT_GRAY, BLACK);
    putString(22, 27, "Press any key to continue...", LIGHT_GREEN, BLACK);
}

void loadTextFile(const string& filename) {
    textLines.clear();
    currentTextFile = filename;
    ifstream file(filename);
    if (file.is_open()) {
        string line;
        while (getline(file, line)) {
            textLines.push_back(line);
        }
        file.close();
    }
    else {
        textLines.push_back("");
    }
    textCursorLine = 0;
    textCursorCol = 0;
    textScrollLine = 0;
}

void saveTextFile(const string& filename) {
    ofstream file(filename);
    if (file.is_open()) {
        for (const string& line : textLines) {
            file << line << "\n";
        }
        file.close();
    }
}

bool executeCommand(const string& cmd) {
    if (cmd.empty()) return false;
    vector<string> parts = splitCommand(cmd);
    if (parts.empty()) return false;
    string command = parts[0];

    if (command == "help") {
        showCommandHelp();
        updateScreen();
        _getch();
        return true;
    }
    else if (command == "--add") {
        if (parts.size() < 2) {
            playErrorSound();
            printStatusBar("ERROR: Usage: --add [filename]", LIGHT_RED);
            return false;
        }
        string name = parts[1];
        if (createFile(name)) {
            playSuccessSound();
            printStatusBar("File created successfully: " + name, LIGHT_GREEN);
            loadFileList();
        }
        else {
            playErrorSound();
            printStatusBar("ERROR: Failed to create file " + name, LIGHT_RED);
        }
        return true;
    }
    else if (command == "--addf") {
        if (parts.size() < 2) {
            playErrorSound();
            printStatusBar("ERROR: Usage: --addf [foldername]", LIGHT_RED);
            return false;
        }
        string name = parts[1];
        if (createFolder(name)) {
            playSuccessSound();
            printStatusBar("Folder created successfully: " + name, LIGHT_GREEN);
            loadFileList();
        }
        else {
            playErrorSound();
            printStatusBar("ERROR: Failed to create folder " + name, LIGHT_RED);
        }
        return true;
    }
    else if (command == "--rename") {
        if (parts.size() < 3) {
            playErrorSound();
            printStatusBar("ERROR: Usage: --rename [old] [new]", LIGHT_RED);
            return false;
        }
        string oldName = parts[1];
        string newName = parts[2];
        if (renameFileOrFolder(oldName, newName)) {
            playSuccessSound();
            printStatusBar("Renamed successfully: " + oldName + " -> " + newName, LIGHT_GREEN);
            loadFileList();
        }
        else {
            playErrorSound();
            printStatusBar("ERROR: Failed to rename", LIGHT_RED);
        }
        return true;
    }
    else if (command == "--delete") {
        if (parts.size() < 2) {
            playErrorSound();
            printStatusBar("ERROR: Usage: --delete [name]", LIGHT_RED);
            return false;
        }
        string name = parts[1];
        if (deleteFileOrFolder(name)) {
            playSuccessSound();
            printStatusBar("Deleted successfully: " + name, LIGHT_GREEN);
            loadFileList();
        }
        else {
            playErrorSound();
            printStatusBar("ERROR: Failed to delete " + name, LIGHT_RED);
        }
        return true;
    }
    else if (command == "--cd") {
        if (parts.size() < 2) {
            playErrorSound();
            printStatusBar("ERROR: Usage: --cd [directory]", LIGHT_RED);
            return false;
        }
        string dir = parts[1];
        changeDirectory(dir);
        loadFileList();
        playSuccessSound();
        printStatusBar("Changed directory to: " + currentDir, LIGHT_GREEN);
        return true;
    }
    else if (command == "--list") {
        listAllFiles();
        playSuccessSound();
        printStatusBar("Listing files in: " + currentDir, LIGHT_GREEN);
        return true;
    }
    else if (command == "--clear") {
        playSuccessSound();
        commandLine = "";
        commandCursorPos = 0;
        printStatusBar("Command console cleared", LIGHT_GREEN);
        return true;
    }
    else if (command == "--clearf") {
        if (parts.size() < 2) {
            playErrorSound();
            printStatusBar("ERROR: Usage: --clearf [foldername]", LIGHT_RED);
            return false;
        }
        string folderName = parts[1];

        markLineDirty(34);
        putString(6, 34, "Clear ALL files in folder '", LIGHT_YELLOW, BLACK);
        putString(6 + 27, 34, folderName, LIGHT_RED, BLACK);
        putString(6 + 27 + folderName.length(), 34, "'? (Y/N): ", LIGHT_YELLOW, BLACK);
        updateScreen();

        char response = _getch();
        if (response == 'y' || response == 'Y') {
            if (clearFolder(folderName)) {
                playSuccessSound();
                printStatusBar("Folder cleared successfully: " + folderName, LIGHT_GREEN);
                loadFileList();
            }
            else {
                playErrorSound();
                printStatusBar("ERROR: Failed to clear folder " + folderName, LIGHT_RED);
            }
        }
        else {
            playMoveSound();
            printStatusBar("Operation cancelled.", LIGHT_GRAY);
        }
        return true;
    }
    playErrorSound();
    printStatusBar("ERROR: Unknown command: " + command, LIGHT_RED);
    return false;
}

void printFileManager() {
    static int lastSelectedFile = -1;
    static string lastCommandLine = "";
    static int lastCommandCursorPos = -1;
    static bool lastBlinkState = false;

    bool fileChanged = (lastSelectedFile != selectedFile);
    bool cmdChanged = (lastCommandLine != commandLine);
    bool cursorChanged = (lastCommandCursorPos != commandCursorPos);
    bool blinkChanged = (lastBlinkState != blinkState);

    if (!needRedraw && !fileChanged && !cmdChanged && !cursorChanged && !blinkChanged) {
        return;
    }

    lastSelectedFile = selectedFile;
    lastCommandLine = commandLine;
    lastCommandCursorPos = commandCursorPos;
    lastBlinkState = blinkState;

    drawBox(2, 4, CONSOLE_WIDTH - 40, 15, CYAN, "File Manager", true);

    putString(4, 6, "Current Directory: ", YELLOW, BLACK);
    char buffer[MAX_PATH];
    GetCurrentDirectoryA(MAX_PATH, buffer);
    currentDir = buffer;
    putString(4 + 19, 6, currentDir, LIGHT_GREEN, BLACK);

    drawBox(4, 7, CONSOLE_WIDTH - 44, 10, BLUE);

    putString(6, 8, "Name", YELLOW, BLACK);
    putString(35, 8, "Type", YELLOW, BLACK);
    putString(50, 8, "Size", YELLOW, BLACK);
    putString(65, 8, "Modified", YELLOW, BLACK);

    for (int i = 0; i < CONSOLE_WIDTH - 50; i++) {
        putChar(6 + i, 9, char(196), LIGHT_GRAY, BLACK);
    }

    int startY = 10;
    int visibleItems = min(8, (int)fileList.size());
    for (int i = 0; i < visibleItems; i++) {
        int idx = i;
        if (selectedFile >= visibleItems) {
            idx = selectedFile - visibleItems + i + 1;
            if (idx >= (int)fileList.size()) idx = (int)fileList.size() - 1;
        }

        if (idx == selectedFile) {
            putChar(6, startY + i, char(16), LIGHT_GREEN, BLACK);
        }
        else {
            putChar(6, startY + i, ' ', LIGHT_GRAY, BLACK);
        }

        string name = fileList[idx].name;
        if (name.length() > 25) name = name.substr(0, 22) + "...";

        if (idx == selectedFile) {
            putString(8, startY + i, name, YELLOW, BLACK);
        }
        else if (fileList[idx].isDirectory) {
            putString(8, startY + i, name, LIGHT_CYAN, BLACK);
        }
        else if (fileList[idx].isTextFile) {
            putString(8, startY + i, name, LIGHT_GREEN, BLACK);
        }
        else if (fileList[idx].isDrawingFile) {
            putString(8, startY + i, name, LIGHT_MAGENTA, BLACK);
        }
        else if (fileList[idx].isMusicFile) {
            putString(8, startY + i, name, LIGHT_YELLOW, BLACK);
        }
        else {
            putString(8, startY + i, name, LIGHT_GRAY, BLACK);
        }

        putString(35, startY + i, fileList[idx].type,
            fileList[idx].isDirectory ? LIGHT_CYAN : LIGHT_GRAY, BLACK);

        if (fileList[idx].size > 0) {
            putString(50, startY + i, formatSize(fileList[idx].size), LIGHT_GRAY, BLACK);
        }

        putString(65, startY + i, fileList[idx].modified, LIGHT_GRAY, BLACK);
    }

    drawBox(4, 18, CONSOLE_WIDTH - 44, 8, MAGENTA, "Command Console", true);

    string displayCmd = commandLine;
    if (showCursor && blinkState) {
        if (commandCursorPos <= (int)displayCmd.length()) {
            displayCmd.insert(commandCursorPos, "_");
        }
    }

    putString(6, 20, "C:\\VirtualDisk\\> " + displayCmd, LIGHT_GRAY, BLACK);

    for (int i = displayCmd.length() + 18; i < CONSOLE_WIDTH - 60; i++) {
        putChar(6 + i, 20, ' ', LIGHT_GRAY, BLACK);
    }

    drawBox(4, 23, CONSOLE_WIDTH - 44, 5, BROWN, "Quick Commands", true);
    putString(6, 25, "--add [file]", LIGHT_GREEN, BLACK);
    putString(30, 25, "--addf [folder]", LIGHT_GREEN, BLACK);
    putString(60, 25, "--delete [name]", LIGHT_GREEN, BLACK);
    putString(6, 26, "--cd [dir]", LIGHT_CYAN, BLACK);
    putString(30, 26, "--list", LIGHT_CYAN, BLACK);
    putString(60, 26, "help", LIGHT_CYAN, BLACK);

    putString(4, 29, "Keys: ", YELLOW, BLACK);
    putString(4 + 6, 29, "ENTER=Execute/Open  ESC=Back  Ctrl+R=Refresh", LIGHT_GRAY, BLACK);
}

bool handleFileManagerMode() {
    clearScreen();
    updateScreen();
    needRedraw = true;
    selectedFile = 0;
    commandLine = "";
    commandCursorPos = 0;
    loadFileList();
    needRedraw = true;

    while (true) {
        getSystemInfo();

        if (clock() - lastBlink > 500) {
            blinkState = !blinkState;
            lastBlink = clock();
        }

        printFileManager();
        updateScreen();

        if (_kbhit()) {
            int key = _getch();
            if (key == 0 || key == 224) {
                key = _getch();
                if (key == 72 && selectedFile > 0) {
                    playMoveSound();
                    selectedFile--;
                }
                if (key == 80 && selectedFile < (int)fileList.size() - 1) {
                    playMoveSound();
                    selectedFile++;
                }
                if (key == 73) {
                    playMoveSound();
                    selectedFile = max(0, selectedFile - 5);
                }
                if (key == 81) {
                    playMoveSound();
                    selectedFile = min((int)fileList.size() - 1, selectedFile + 5);
                }
                if (key == 75 && commandCursorPos > 0) {
                    playMoveSound();
                    commandCursorPos--;
                }
                if (key == 77 && commandCursorPos < (int)commandLine.length()) {
                    playMoveSound();
                    commandCursorPos++;
                }
            }
            else if (key == 13) {
                playSelectSound();
                if (!commandLine.empty()) {
                    addToCommandHistory(commandLine);
                    executeCommand(commandLine);
                    commandLine = "";
                    commandCursorPos = 0;
                    needRedraw = true;
                }
                else {
                    if (!fileList.empty() && selectedFile < (int)fileList.size()) {
                        if (fileList[selectedFile].name == "..") {
                            changeDirectory("..");
                            selectedFile = 0;
                            loadFileList();
                            needRedraw = true;
                        }
                        else if (fileList[selectedFile].isDirectory) {
                            changeDirectory(fileList[selectedFile].name);
                            selectedFile = 0;
                            loadFileList();
                            needRedraw = true;
                        }
                        else if (fileList[selectedFile].isTextFile) {
                            string filename = fileList[selectedFile].name;
                            string fullPath;

                            char buffer[MAX_PATH];
                            GetCurrentDirectoryA(MAX_PATH, buffer);
                            string currentPath(buffer);

                            if (currentDir == ".") {
                                fullPath = currentPath + "\\" + filename;
                            }
                            else {
                                fullPath = currentDir + "\\" + filename;
                            }

                            loadTextFile(fullPath);
                            currentScreen = TEXT_EDITOR;
                            return true;
                        }
                        else if (fileList[selectedFile].isDrawingFile) {
                            string filename = fileList[selectedFile].name;
                            string fullPath;

                            char buffer[MAX_PATH];
                            GetCurrentDirectoryA(MAX_PATH, buffer);
                            string currentPath(buffer);

                            if (currentDir == ".") {
                                fullPath = currentPath + "\\" + filename;
                            }
                            else {
                                fullPath = currentDir + "\\" + filename;
                            }

                            if (loadPaintDrawing(fullPath)) {
                                playOpenSound();
                                currentScreen = PAINT;
                                return true;
                            }
                            else {
                                playErrorSound();
                                printStatusBar("ERROR: Failed to load drawing file", LIGHT_RED);
                                _getch();
                            }
                        }
                        else if (fileList[selectedFile].isMusicFile) {
                            string filename = fileList[selectedFile].name;
                            string fullPath;

                            char buffer[MAX_PATH];
                            GetCurrentDirectoryA(MAX_PATH, buffer);
                            string currentPath(buffer);

                            if (currentDir == ".") {
                                fullPath = currentPath + "\\" + filename;
                            }
                            else {
                                fullPath = currentDir + "\\" + filename;
                            }

                            if (loadMusicComposition(fullPath)) {
                                currentScreen = MUSIC_EDITOR;
                                return true;
                            }
                            else {
                                playErrorSound();
                                printStatusBar("ERROR: Failed to load music file", LIGHT_RED);
                                _getch();
                            }
                        }
                        else {
                            playErrorSound();
                            printStatusBar("ERROR: Unknown file type or cannot open", LIGHT_RED);
                            _getch();
                        }
                    }
                }
            }
            else if (key == 27) {
                playCloseSound();
                currentScreen = DESKTOP;
                return true;
            }
            else if ((key == 8) && (GetKeyState(VK_CONTROL) & 0x8000)) {
                playSelectSound();
                currentScreen = HELP;
                return true;
            }
            else if ((key == 18) && (GetKeyState(VK_CONTROL) & 0x8000)) {
                playSelectSound();
                loadFileList();
                printStatusBar("File list refreshed", LIGHT_GREEN);
                needRedraw = true;
            }
            else if ((key == 16) && (GetKeyState(VK_CONTROL) & 0x8000)) {
                playSelectSound();
                systemMonitorVisible = !systemMonitorVisible;
                needRedraw = true;
            }
            else if (key == 8 && commandCursorPos > 0) {
                playMoveSound();
                commandCursorPos--;
                commandLine.erase(commandCursorPos, 1);
            }
            else if (isprint(key)) {
                playMoveSound();
                commandLine.insert(commandCursorPos, 1, char(key));
                commandCursorPos++;
            }
        }
        Sleep(50);
    }
    return false;
}

void printTextEditor() {
    static int lastTextCursorLine = -1;
    static int lastTextCursorCol = -1;
    static int lastTextScrollLine = -1;
    static bool lastBlinkState = false;

    bool cursorChanged = (lastTextCursorLine != textCursorLine ||
        lastTextCursorCol != textCursorCol);
    bool scrollChanged = (lastTextScrollLine != textScrollLine);
    bool blinkChanged = (lastBlinkState != blinkState);

    if (!needRedraw && !cursorChanged && !scrollChanged && !blinkChanged) {
        return;
    }

    lastTextCursorLine = textCursorLine;
    lastTextCursorCol = textCursorCol;
    lastTextScrollLine = textScrollLine;
    lastBlinkState = blinkState;

    drawBox(2, 4, CONSOLE_WIDTH - 40, 30, MAGENTA, "Text Editor", true);

    putString(4, 6, "Editing: ", YELLOW, BLACK);
    putString(4 + 9, 6, currentTextFile, LIGHT_GREEN, BLACK);

    drawBox(4, 7, CONSOLE_WIDTH - 44, 20, BLACK);

    int visibleLines = min(18, (int)textLines.size());
    for (int i = 0; i < visibleLines; i++) {
        int lineIdx = textScrollLine + i;
        if (lineIdx >= (int)textLines.size()) break;

        if (lineIdx == textCursorLine) {
            putChar(6, 9 + i, '>', LIGHT_GREEN, BLACK);
        }
        else {
            putChar(6, 9 + i, ' ', LIGHT_GRAY, BLACK);
        }

        string lineNum = to_string(lineIdx + 1);
        if (lineNum.length() < 3) lineNum = string(3 - lineNum.length(), ' ') + lineNum;
        putString(7, 9 + i, lineNum + " | ", LIGHT_GRAY, BLACK);

        string displayLine = textLines[lineIdx];
        if (lineIdx == textCursorLine) {
            if (blinkState) {
                if (textCursorCol < (int)displayLine.length()) {
                    displayLine.insert(textCursorCol, "_");
                }
                else {
                    displayLine += "_";
                }
            }
            if (displayLine.length() > CONSOLE_WIDTH - 53) {
                int startPos = max(0, textCursorCol - 40);
                if (startPos + (CONSOLE_WIDTH - 53) < (int)displayLine.length()) {
                    displayLine = displayLine.substr(startPos, CONSOLE_WIDTH - 53);
                }
                else {
                    displayLine = displayLine.substr(startPos);
                }
            }
            putString(11, 9 + i, displayLine, YELLOW, BLACK);
        }
        else {
            if (displayLine.length() > CONSOLE_WIDTH - 53) {
                displayLine = displayLine.substr(0, CONSOLE_WIDTH - 53) + "...";
            }
            putString(11, 9 + i, displayLine, LIGHT_GRAY, BLACK);
        }
    }

    drawBox(4, 28, CONSOLE_WIDTH - 44, 6, CYAN, "Editor Commands", true);
    putString(6, 30, "CTRL+S: Save", LIGHT_GREEN, BLACK);
    putString(24, 30, "CTRL+Q: Save As", LIGHT_GREEN, BLACK);
    putString(44, 30, "ESC: Back", LIGHT_RED, BLACK);

    putString(6, 31, "Lines: ", YELLOW, BLACK);
    putString(6 + 7, 31, to_string(textLines.size()), LIGHT_GRAY, BLACK);

    putString(20, 31, "Cursor: ", YELLOW, BLACK);
    putString(20 + 8, 31, "L" + to_string(textCursorLine + 1) + ":C" + to_string(textCursorCol + 1), LIGHT_GRAY, BLACK);

    putString(40, 31, "Size: ", YELLOW, BLACK);
    long totalChars = 0;
    for (const auto& line : textLines) totalChars += (long)line.length();
    putString(40 + 6, 31, to_string(totalChars) + " chars", LIGHT_GRAY, BLACK);

    putString(4, 35, "Keys: ", YELLOW, BLACK);
    putString(4 + 6, 35, "ARROWS=Navigate  CTRL+S=Save  DEL=Delete  BACKSPACE=Back", LIGHT_GRAY, BLACK);
}

bool handleTextEditorMode() {
    clearScreen();
    updateScreen();
    needRedraw = true;
    needRedraw = true;

    while (true) {
        getSystemInfo();

        if (clock() - lastBlink > 500) {
            blinkState = !blinkState;
            lastBlink = clock();
        }

        printTextEditor();
        updateScreen();

        if (_kbhit()) {
            int key = _getch();
            if (key == 0 || key == 224) {
                key = _getch();
                if (key == 72 && textCursorLine > 0) {
                    playMoveSound();
                    textCursorLine--;
                    textCursorCol = min(textCursorCol, (int)textLines[textCursorLine].length());
                    if (textCursorLine < textScrollLine) {
                        textScrollLine = textCursorLine;
                    }
                }
                if (key == 80 && textCursorLine < (int)textLines.size() - 1) {
                    playMoveSound();
                    textCursorLine++;
                    textCursorCol = min(textCursorCol, (int)textLines[textCursorLine].length());
                    if (textCursorLine >= textScrollLine + 18) {
                        textScrollLine = textCursorLine - 17;
                    }
                }
                if (key == 75 && textCursorCol > 0) {
                    playMoveSound();
                    textCursorCol--;
                }
                if (key == 77 && textCursorCol < (int)textLines[textCursorLine].length()) {
                    playMoveSound();
                    textCursorCol++;
                }
                if (key == 83) {
                    playMoveSound();
                    if (textCursorCol < (int)textLines[textCursorLine].length()) {
                        textLines[textCursorLine].erase(textCursorCol, 1);
                    }
                    else if (textCursorLine < (int)textLines.size() - 1) {
                        textLines[textCursorLine] += textLines[textCursorLine + 1];
                        textLines.erase(textLines.begin() + textCursorLine + 1);
                    }
                }
            }
            else if (key == 27) {
                playCloseSound();
                currentScreen = FILE_MANAGER;
                return true;
            }
            else if ((key == 19) && (GetKeyState(VK_CONTROL) & 0x8000)) {
                playSelectSound();
                saveTextFile(currentTextFile);
                playSuccessSound();
                printStatusBar("File saved: " + currentTextFile, LIGHT_GREEN);
                _getch();
            }
            else if ((key == 8) && (GetKeyState(VK_CONTROL) & 0x8000)) {
                playSelectSound();
                currentScreen = HELP;
                return true;
            }
            else if ((key == 16) && (GetKeyState(VK_CONTROL) & 0x8000)) {
                playSelectSound();
                systemMonitorVisible = !systemMonitorVisible;
                needRedraw = true;
            }
            else if (key == 8) {
                playMoveSound();
                if (textCursorCol > 0) {
                    textLines[textCursorLine].erase(textCursorCol - 1, 1);
                    textCursorCol--;
                }
                else if (textCursorLine > 0) {
                    string currentLine = textLines[textCursorLine];
                    textLines.erase(textLines.begin() + textCursorLine);
                    textCursorLine--;
                    textCursorCol = (int)textLines[textCursorLine].length();
                    textLines[textCursorLine] += currentLine;
                }
            }
            else if (key == 13) {
                playMoveSound();
                string leftPart = textLines[textCursorLine].substr(0, textCursorCol);
                string rightPart = textLines[textCursorLine].substr(textCursorCol);
                textLines[textCursorLine] = leftPart;
                textLines.insert(textLines.begin() + textCursorLine + 1, rightPart);
                textCursorLine++;
                textCursorCol = 0;
                if (textCursorLine >= textScrollLine + 18) {
                    textScrollLine++;
                }
            }
            else if (isprint(key)) {
                playMoveSound();
                textLines[textCursorLine].insert(textCursorCol, 1, char(key));
                textCursorCol++;
            }
        }
        Sleep(50);
    }
    return false;
}

void printHelpSystem() {
    if (!needRedraw) return;
    needRedraw = false;

    drawBox(2, 4, CONSOLE_WIDTH - 40, 30, BROWN, "Help System", true);

    vector<pair<string, string>> helpItems = {
        {"SOSIOC v1.0:", "Desktop Operating System"},
        {"Developer:", "SOSIOC Systems Corporation"},
        {"Release:", "2025"},
        {"", ""},
        {"System Requirements:", ""},
        {"- CPU:", "Intel 8086 or compatible"},
        {"- RAM:", "640 KB minimum"},
        {"- Display:", "VGA 80x25 text mode"},
        {"- Storage:", "20 MB free space"},
        {"", ""},
        {"Applications:", ""},
        {"1. Calculator:", "Advanced calculator with variables"},
        {"2. File Manager:", "Browse and manage files"},
        {"3. Text Editor:", "Simple text editor"},
        {"4. Settings:", "System configuration"},
        {"5. Task Manager:", "Manage your tasks"},
        {"6. Memory Game:", "Test your memory"},
        {"7. Process Monitor:", "Monitor system processes"},
        {"8. Paint:", "Simple drawing program"},
        {"9. Music Player:", "Play music files"},
        {"10. Weather:", "Weather forecast"},
        {"11. Arcade Game:", "Snake arcade game"},
        {"12. Clock:", "Digital and analog clock"},
        {"13. New Year Quest:", "Holiday adventure game"},
        {"14. Music Editor:", "Create and edit music compositions"},
        {"15. Racing Game:", "Arcade racing game"},
        {"16. C++ Compiler:", "Compile and run C++ code"},
        {"", ""},
        {"Hotkeys:", ""},
        {"Ctrl+H:", "This help screen"},
        {"Ctrl+P:", "Toggle system panel"},
        {"ESC:", "Back/Exit"},
        {"Ctrl+R:", "Refresh (File Manager)"},
        {"Ctrl+S:", "Save (Text Editor, Paint, Music Editor)"},
        {"", ""},
        {"Navigation:", "Arrow keys and ENTER"},
        {"Scrolling:", "PGUP/PGDN for app list"},
        {"", ""},
        {"Virtual Disk:", "All files stored in C:\\SOSIOC\\main"}
    };

    int y = 6;
    for (const auto& item : helpItems) {
        if (!item.first.empty()) {
            if (item.first.find(":") != string::npos) {
                putString(4, y, item.first, YELLOW, BLACK);
                putString(4 + item.first.length() + 1, y, item.second, LIGHT_GRAY, BLACK);
            }
            else {
                putString(4, y, item.first, LIGHT_CYAN, BLACK);
            }
        }
        else {
            putString(4, y, item.second, LIGHT_GRAY, BLACK);
        }
        y++;
    }

    putString(4, 34, "Press ESC to return to desktop", YELLOW, BLACK);
}

bool handleHelpMode() {
    clearScreen();
    updateScreen();
    needRedraw = true;
    needRedraw = true;

    while (true) {
        getSystemInfo();
        printHelpSystem();
        updateScreen();

        if (_kbhit()) {
            int key = _getch();
            if (key == 27) {
                playCloseSound();
                currentScreen = DESKTOP;
                return true;
            }
            else if ((key == 16) && (GetKeyState(VK_CONTROL) & 0x8000)) {
                playSelectSound();
                systemMonitorVisible = !systemMonitorVisible;
                needRedraw = true;
            }
        }
        Sleep(50);
    }
    return false;
}

void printSettings() {
    if (!needRedraw) return;
    needRedraw = false;

    drawBox(2, 4, CONSOLE_WIDTH - 40, 30, LIGHT_BLUE, "System Settings", true);

    putString(4, 6, "System Configuration", YELLOW, BLACK);
    drawBox(4, 7, CONSOLE_WIDTH - 44, 22, BLUE);

    vector<pair<string, string>> settings = {
        {"Display Settings:", ""},
        {"- Current Font:", systemSettings.fontName},
        {"- Font Size:", to_string(systemSettings.fontSizeX) + "x" + to_string(systemSettings.fontSizeY)},
        {"- Screen Size:", to_string(CONSOLE_WIDTH) + "x" + to_string(CONSOLE_HEIGHT)},
        {"- Color Mode:", "16-color VGA"},
        {"", ""},
        {"System Settings:", ""},
        {"- History Limit:", to_string(HISTORY_LIMIT) + " entries"},
        {"- Applications:", to_string(applications.size())},
        {"- Uptime:", sysInfo.uptime},
        {"", ""},
        {"Memory Settings:", ""},
        {"- Total RAM:", to_string(sysInfo.totalMemory) + " MB"},
        {"- Free RAM:", to_string(sysInfo.freeMemory) + " MB"},
        {"- App Usage:", to_string(sysInfo.appMemory).substr(0, 4) + " MB"},
        {"", ""},
        {"Virtual Disk:", ""},
        {"- Location:", "C:\\SOSIOC\\main"},
        {"- Folders:", "drawings, music, other, bin, test, cpp"},
        {"- Status:", "Active"},
        {"", ""},
        {"Application Info:", ""},
        {"- Calculator:", "Ready"},
        {"- File Manager:", "Ready"},
        {"- Text Editor:", "Ready"},
        {"- Task Manager:", "Ready"},
        {"- Memory Game:", "Ready"},
        {"- Process Monitor:", "Ready"},
        {"- Paint:", "Ready"},
        {"- Music Player:", "Ready"},
        {"- Music Editor:", "Ready"},
        {"- Weather:", "Ready"},
        {"- Arcade Game:", "Ready"},
        {"- Racing Game:", "Ready"},
        {"- C++ Compiler:", "Ready"},
        {"- Clock:", "Ready"},
        {"- New Year Quest:", "Ready"},
        {"- Help System:", "Ready"},
        {"", ""},
        {"Version Information:", ""},
        {"- SOSIOC:", "v1.0 (2025)"},
        {"- Kernel:", "MS-DOS 6.22 compatible"},
        {"- Shell:", "COMMAND.COM replacement"}
    };

    int y = 8;
    for (const auto& item : settings) {
        if (!item.first.empty()) {
            if (item.first.find(":") != string::npos) {
                if (item.first.find("- ") == 0) {
                    putString(6, y, item.first, LIGHT_GRAY, BLACK);
                    putString(6 + item.first.length() + 1, y, item.second, LIGHT_GREEN, BLACK);
                }
                else {
                    putString(6, y, item.first, YELLOW, BLACK);
                    putString(6 + item.first.length() + 1, y, item.second, LIGHT_GRAY, BLACK);
                }
            }
            else {
                putString(6, y, item.first, LIGHT_CYAN, BLACK);
            }
        }
        else {
            putString(6, y, item.second, LIGHT_GRAY, BLACK);
        }
        y++;
    }

    drawBox(4, 30, CONSOLE_WIDTH - 44, 5, GREEN, "Quick Actions", true);
    putString(6, 32, "F1: Terminal Font (8x16)", LIGHT_GREEN, BLACK);
    putString(40, 32, "F2: Consolas Font (8x18)", LIGHT_GREEN, BLACK);
    putString(74, 32, "ESC: Back", YELLOW, BLACK);
}

bool handleSettingsMode() {
    clearScreen();
    updateScreen();
    needRedraw = true;
    needRedraw = true;

    while (true) {
        getSystemInfo();
        printSettings();
        updateScreen();

        if (_kbhit()) {
            int key = _getch();
            if (key == 27) {
                playCloseSound();
                currentScreen = DESKTOP;
                return true;
            }
            else if (key == 59) {
                playSelectSound();
                systemSettings.fontName = "Terminal";
                systemSettings.fontSizeX = 8;
                systemSettings.fontSizeY = 16;
                setFont("Terminal", 8, 16);
                printStatusBar("Font changed to Terminal (8x16)", LIGHT_GREEN);
                _getch();
                needRedraw = true;
            }
            else if (key == 60) {
                playSelectSound();
                systemSettings.fontName = "Consolas";
                systemSettings.fontSizeX = 8;
                systemSettings.fontSizeY = 18;
                setFont("Consolas", 8, 18);
                printStatusBar("Font changed to Consolas (8x18)", LIGHT_GREEN);
                _getch();
                needRedraw = true;
            }
            else if ((key == 18) && (GetKeyState(VK_CONTROL) & 0x8000)) {
                playSelectSound();
                printStatusBar("System restarting...", YELLOW);
                _getch();
                return false;
            }
            else if ((key == 19) && (GetKeyState(VK_CONTROL) & 0x8000)) {
                playSelectSound();
                printStatusBar("System shutting down...", YELLOW);
                _getch();
                return false;
            }
            else if ((key == 8) && (GetKeyState(VK_CONTROL) & 0x8000)) {
                playSelectSound();
                currentScreen = HELP;
                return true;
            }
            else if ((key == 16) && (GetKeyState(VK_CONTROL) & 0x8000)) {
                playSelectSound();
                systemMonitorVisible = !systemMonitorVisible;
                needRedraw = true;
            }
        }
        Sleep(50);
    }
    return false;
}

void printTaskManagerApp() {
    static int lastSelectedTask = -1;
    static bool lastTaskEditMode = false;
    static bool lastEditingName = false;

    bool selectionChanged = (lastSelectedTask != selectedTask);
    bool editModeChanged = (lastTaskEditMode != taskEditMode);
    bool editingChanged = (lastEditingName != editingName);

    if (!needRedraw && !selectionChanged && !editModeChanged && !editingChanged) {
        return;
    }

    lastSelectedTask = selectedTask;
    lastTaskEditMode = taskEditMode;
    lastEditingName = editingName;

    drawBox(2, 4, CONSOLE_WIDTH - 40, 30, LIGHT_MAGENTA, "Task Manager", true);

    putString(4, 6, "Your Tasks:", YELLOW, BLACK);
    drawBox(4, 7, CONSOLE_WIDTH - 44, 20, BLUE);

    putString(6, 8, "ID", YELLOW, BLACK);
    putString(12, 8, "Task Name", YELLOW, BLACK);
    putString(35, 8, "Description", YELLOW, BLACK);
    putString(70, 8, "Status", YELLOW, BLACK);

    for (int i = 0; i < CONSOLE_WIDTH - 50; i++) {
        putChar(6 + i, 9, char(196), LIGHT_GRAY, BLACK);
    }

    int startY = 10;
    int visibleItems = min(15, (int)tasks.size());
    for (int i = 0; i < visibleItems; i++) {
        int idx = i;
        if (selectedTask >= visibleItems) {
            idx = selectedTask - visibleItems + i + 1;
            if (idx >= (int)tasks.size()) idx = (int)tasks.size() - 1;
        }

        if (idx == selectedTask) {
            putChar(6, startY + i, char(16), LIGHT_GREEN, BLACK);
        }
        else {
            putChar(6, startY + i, ' ', LIGHT_GRAY, BLACK);
        }

        putString(8, startY + i, to_string(tasks[idx].id), LIGHT_GRAY, BLACK);

        string displayName = tasks[idx].name;
        if (displayName.length() > 20) displayName = displayName.substr(0, 17) + "...";

        if (taskEditMode && idx == selectedTask && editingName) {
            if (blinkState) {
                putString(12, startY + i, displayName + "_", YELLOW, BLACK);
            }
            else {
                putString(12, startY + i, displayName + " ", YELLOW, BLACK);
            }
        }
        else if (idx == selectedTask) {
            putString(12, startY + i, displayName, YELLOW, BLACK);
        }
        else {
            putString(12, startY + i, displayName, LIGHT_GRAY, BLACK);
        }

        string desc = tasks[idx].description;
        if (desc.length() > 30) desc = desc.substr(0, 27) + "...";

        if (taskEditMode && idx == selectedTask && !editingName) {
            if (blinkState) {
                putString(35, startY + i, desc + "_", YELLOW, BLACK);
            }
            else {
                putString(35, startY + i, desc + " ", YELLOW, BLACK);
            }
        }
        else if (idx == selectedTask) {
            putString(35, startY + i, desc, YELLOW, BLACK);
        }
        else {
            putString(35, startY + i, desc, LIGHT_GRAY, BLACK);
        }

        if (tasks[idx].completed) {
            putString(70, startY + i, "[COMPLETED]", LIGHT_GREEN, BLACK);
        }
        else {
            putString(70, startY + i, "[PENDING]", LIGHT_RED, BLACK);
        }
    }

    drawBox(4, 28, CONSOLE_WIDTH - 44, 6, GREEN, "Task Commands", true);

    if (taskEditMode) {
        putString(6, 30, "EDIT MODE: Type text, BACKSPACE to delete", LIGHT_CYAN, BLACK);
        putString(6, 31, "ENTER: Save  ESC: Cancel  TAB: Switch field", LIGHT_CYAN, BLACK);
    }
    else {
        putString(6, 30, "ENTER: Edit Task", LIGHT_GREEN, BLACK);
        putString(30, 30, "Ctrl+T: Toggle Complete", LIGHT_GREEN, BLACK);
        putString(60, 30, "Ctrl+A: Add  Ctrl+D: Delete", LIGHT_GREEN, BLACK);
        putString(6, 31, "ESC: Back", YELLOW, BLACK);
    }

    putString(6, 32, "Total Tasks: ", YELLOW, BLACK);
    putString(6 + 13, 32, to_string(tasks.size()), LIGHT_GRAY, BLACK);

    int completed = 0;
    for (const auto& task : tasks) {
        if (task.completed) completed++;
    }

    putString(30, 32, "Completed: ", YELLOW, BLACK);
    putString(30 + 11, 32, to_string(completed) + "/" + to_string(tasks.size()), LIGHT_GRAY, BLACK);

    putString(60, 32, "Progress: ", YELLOW, BLACK);
    if (tasks.size() > 0) {
        int percent = (completed * 100) / (int)tasks.size();
        putString(60 + 10, 32, to_string(percent) + "%", LIGHT_GRAY, BLACK);
    }
    else {
        putString(60 + 10, 32, "0%", LIGHT_GRAY, BLACK);
    }

    putString(4, 35, "Keys: ", YELLOW, BLACK);
    if (taskEditMode) {
        putString(4 + 6, 35, "Type text, BACKSPACE=Delete, ENTER=Save, ESC=Cancel, TAB=Switch", LIGHT_GRAY, BLACK);
    }
    else {
        putString(4 + 6, 35, "ARROWS=Navigate  ENTER=Edit  Ctrl+T=Toggle  Ctrl+A=Add  Ctrl+D=Delete  ESC=Back", LIGHT_GRAY, BLACK);
    }
}

bool handleTaskManagerMode() {
    clearScreen();
    updateScreen();
    needRedraw = true;
    selectedTask = 0;
    taskEditMode = false;
    editingName = true;
    initializeTasks();
    needRedraw = true;

    while (true) {
        getSystemInfo();

        if (clock() - lastBlink > 500) {
            blinkState = !blinkState;
            lastBlink = clock();
        }

        printTaskManagerApp();
        updateScreen();

        if (_kbhit()) {
            int key = _getch();
            if (taskEditMode) {
                if (key == 9) {
                    playMoveSound();
                    editingName = !editingName;
                }
                else if (key == 13) {
                    playSelectSound();
                    taskEditMode = false;
                }
                else if (key == 27) {
                    playCloseSound();
                    taskEditMode = false;
                }
                else if (key == 8) {
                    playMoveSound();
                    if (editingName) {
                        if (!tasks[selectedTask].name.empty()) {
                            tasks[selectedTask].name.pop_back();
                        }
                    }
                    else {
                        if (!tasks[selectedTask].description.empty()) {
                            tasks[selectedTask].description.pop_back();
                        }
                    }
                }
                else if (isprint(key)) {
                    playMoveSound();
                    if (editingName) {
                        if (tasks[selectedTask].name.length() < 50) {
                            tasks[selectedTask].name += char(key);
                        }
                    }
                    else {
                        if (tasks[selectedTask].description.length() < 100) {
                            tasks[selectedTask].description += char(key);
                        }
                    }
                }
            }
            else {
                if (key == 0 || key == 224) {
                    key = _getch();
                    if (key == 72 && selectedTask > 0) {
                        playMoveSound();
                        selectedTask--;
                    }
                    if (key == 80 && selectedTask < (int)tasks.size() - 1) {
                        playMoveSound();
                        selectedTask++;
                    }
                    if (key == 73) {
                        playMoveSound();
                        selectedTask = max(0, selectedTask - 5);
                    }
                    if (key == 81) {
                        playMoveSound();
                        selectedTask = min((int)tasks.size() - 1, selectedTask + 5);
                    }
                }
                else if ((key == 20) && (GetKeyState(VK_CONTROL) & 0x8000)) {
                    playSelectSound();
                    if (selectedTask < (int)tasks.size()) {
                        tasks[selectedTask].completed = !tasks[selectedTask].completed;
                    }
                }
                else if (key == 13) {
                    playSelectSound();
                    if (!tasks.empty() && selectedTask < (int)tasks.size()) {
                        taskEditMode = true;
                        editingName = true;
                    }
                }
                else if ((key == 1) && (GetKeyState(VK_CONTROL) & 0x8000)) {
                    playSelectSound();
                    if (tasks.size() < 20) {
                        Task newTask;
                        newTask.id = tasks.empty() ? 1 : tasks.back().id + 1;
                        newTask.name = "New Task " + to_string(newTask.id);
                        newTask.description = "Description for task " + to_string(newTask.id);
                        newTask.completed = false;
                        tasks.push_back(newTask);
                        selectedTask = (int)tasks.size() - 1;
                    }
                }
                else if ((key == 4) && (GetKeyState(VK_CONTROL) & 0x8000)) {
                    playSelectSound();
                    if (!tasks.empty() && selectedTask < (int)tasks.size()) {
                        tasks.erase(tasks.begin() + selectedTask);
                        if (selectedTask >= (int)tasks.size() && !tasks.empty()) {
                            selectedTask = (int)tasks.size() - 1;
                        }
                    }
                }
                else if (key == 27) {
                    playCloseSound();
                    currentScreen = DESKTOP;
                    return true;
                }
                else if ((key == 8) && (GetKeyState(VK_CONTROL) & 0x8000)) {
                    playSelectSound();
                    currentScreen = HELP;
                    return true;
                }
                else if ((key == 16) && (GetKeyState(VK_CONTROL) & 0x8000)) {
                    playSelectSound();
                    systemMonitorVisible = !systemMonitorVisible;
                    needRedraw = true;
                }
            }
        }
        Sleep(50);
    }
    return false;
}

void printMemoryGame() {
    static int lastSelectedCard = -1;
    static int lastFirstCard = -2;
    static int lastSecondCard = -2;
    static int lastMatchedPairs = -1;
    static int lastMoves = -1;

    bool cardChanged = (lastSelectedCard != selectedCard);
    bool gameChanged = (lastFirstCard != firstCard || lastSecondCard != secondCard);
    bool scoreChanged = (lastMatchedPairs != matchedPairs || lastMoves != moves);

    if (!needRedraw && !cardChanged && !gameChanged && !scoreChanged) {
        return;
    }

    lastSelectedCard = selectedCard;
    lastFirstCard = firstCard;
    lastSecondCard = secondCard;
    lastMatchedPairs = matchedPairs;
    lastMoves = moves;

    drawBox(2, 4, CONSOLE_WIDTH - 40, 30, LIGHT_CYAN, "Memory Game", true);

    putString(4, 6, "Find matching pairs of cards!", YELLOW, BLACK);

    for (int i = 0; i < 16; i++) {
        MemoryCard& card = memoryCards[i];
        string cardStr = string("[") + (card.value < 10 ? " " : "") + to_string(card.value) + "]";

        if (card.matched) {
            putString(card.x, card.y, cardStr, WHITE, GREEN);
        }
        else if (card.revealed) {
            putString(card.x, card.y, cardStr, YELLOW, BLUE);
        }
        else {
            if (i == selectedCard) {
                putString(card.x, card.y, "[??]", BLACK, YELLOW);
            }
            else {
                putString(card.x, card.y, "[??]", WHITE, RED);
            }
        }
    }

    drawBox(10, 20, 100, 10, MAGENTA, "Game Info", true);

    putString(12, 22, "Moves: ", YELLOW, BLACK);
    putString(12 + 7, 22, to_string(moves), LIGHT_GRAY, BLACK);

    putString(30, 22, "Pairs Found: ", YELLOW, BLACK);
    putString(30 + 13, 22, to_string(matchedPairs) + "/8", LIGHT_GRAY, BLACK);

    if (gameStarted) {
        int elapsed = (int)((clock() - gameStartTime) / CLOCKS_PER_SEC);
        putString(60, 22, "Time: ", YELLOW, BLACK);
        putString(60 + 6, 22, to_string(elapsed) + "s", LIGHT_GRAY, BLACK);
    }

    putString(12, 24, "Status: ", YELLOW, BLACK);
    if (matchedPairs == 8) {
        putString(12 + 8, 24, "YOU WIN! All pairs found!", LIGHT_GREEN, BLACK);
    }
    else if (firstCard != -1 && secondCard != -1) {
        putString(12 + 8, 24, "Checking match...", LIGHT_CYAN, BLACK);
    }
    else {
        putString(12 + 8, 24, "Select a card", LIGHT_GREEN, BLACK);
    }

    drawBox(10, 26, 100, 6, BROWN, "Game Controls", true);
    putString(12, 28, "ARROWS: Move", LIGHT_GREEN, BLACK);
    putString(35, 28, "ENTER: Reveal Card", LIGHT_GREEN, BLACK);
    putString(65, 28, "Ctrl+R: Restart", LIGHT_CYAN, BLACK);
    putString(90, 28, "ESC: Back", LIGHT_RED, BLACK);

    putString(4, 35, "Keys: ", YELLOW, BLACK);
    putString(4 + 6, 35, "ARROWS=Move  ENTER=Select  Ctrl+R=Restart  ESC=Back", LIGHT_GRAY, BLACK);
}

bool handleMemoryGameMode() {
    clearScreen();
    updateScreen();
    needRedraw = true;
    initializeMemoryGame();
    needRedraw = true;

    while (true) {
        getSystemInfo();
        printMemoryGame();
        updateScreen();

        if (_kbhit()) {
            int key = _getch();
            if (key == 0 || key == 224) {
                key = _getch();
                if (key == 72) {
                    playMoveSound();
                    if (selectedCard >= 8) selectedCard -= 8;
                }
                if (key == 80) {
                    playMoveSound();
                    if (selectedCard < 8) selectedCard += 8;
                }
                if (key == 75) {
                    playMoveSound();
                    if (selectedCard % 8 > 0) selectedCard--;
                }
                if (key == 77) {
                    playMoveSound();
                    if (selectedCard % 8 < 7) selectedCard++;
                }
            }
            else if (key == 13) {
                playSelectSound();
                if (matchedPairs == 8) continue;
                if (!memoryCards[selectedCard].revealed && !memoryCards[selectedCard].matched) {
                    if (!gameStarted) {
                        gameStarted = true;
                        gameStartTime = clock();
                    }
                    memoryCards[selectedCard].revealed = true;
                    if (firstCard == -1) {
                        firstCard = selectedCard;
                    }
                    else if (secondCard == -1 && selectedCard != firstCard) {
                        secondCard = selectedCard;
                        moves++;
                        if (memoryCards[firstCard].value == memoryCards[secondCard].value) {
                            playSuccessSound();
                            memoryCards[firstCard].matched = true;
                            memoryCards[secondCard].matched = true;
                            matchedPairs++;
                            firstCard = -1;
                            secondCard = -1;
                        }
                        else {
                            printMemoryGame();
                            updateScreen();
                            Sleep(1000);
                            memoryCards[firstCard].revealed = false;
                            memoryCards[secondCard].revealed = false;
                            firstCard = -1;
                            secondCard = -1;
                        }
                    }
                }
            }
            else if ((key == 18) && (GetKeyState(VK_CONTROL) & 0x8000)) {
                playSelectSound();
                initializeMemoryGame();
                needRedraw = true;
            }
            else if (key == 27) {
                playCloseSound();
                currentScreen = DESKTOP;
                return true;
            }
            else if ((key == 8) && (GetKeyState(VK_CONTROL) & 0x8000)) {
                playSelectSound();
                currentScreen = HELP;
                return true;
            }
            else if ((key == 16) && (GetKeyState(VK_CONTROL) & 0x8000)) {
                playSelectSound();
                systemMonitorVisible = !systemMonitorVisible;
                needRedraw = true;
            }
        }
        Sleep(50);
    }
    return false;
}

void printProcessMonitor() {
    static int lastSelectedProcess = -1;
    static int lastProcessScrollOffset = -1;

    bool selectionChanged = (lastSelectedProcess != selectedProcess);
    bool scrollChanged = (lastProcessScrollOffset != processScrollOffset);

    if (!needRedraw && !selectionChanged && !scrollChanged) {
        return;
    }

    lastSelectedProcess = selectedProcess;
    lastProcessScrollOffset = processScrollOffset;

    drawBox(2, 4, CONSOLE_WIDTH - 40, 30, LIGHT_RED, "Process Monitor", true);

    putString(4, 6, "System Processes:", YELLOW, BLACK);
    putString(4 + 18, 6, " Total: " + to_string(processes.size()), LIGHT_GREEN, BLACK);

    drawBox(4, 7, CONSOLE_WIDTH - 44, 20, BLUE);

    putString(6, 8, "PID", YELLOW, BLACK);
    putString(12, 8, "Process Name", YELLOW, BLACK);
    putString(40, 8, "Memory (KB)", YELLOW, BLACK);
    putString(60, 8, "Status", YELLOW, BLACK);

    for (int i = 0; i < CONSOLE_WIDTH - 50; i++) {
        putChar(6 + i, 9, char(196), LIGHT_GRAY, BLACK);
    }

    int startY = 10;
    int visibleItems = min(15, (int)processes.size());
    for (int i = 0; i < visibleItems; i++) {
        int idx = i + processScrollOffset;
        if (idx >= (int)processes.size()) break;

        if (idx == selectedProcess) {
            putChar(6, startY + i, char(16), LIGHT_GREEN, BLACK);
        }
        else {
            putChar(6, startY + i, ' ', LIGHT_GRAY, BLACK);
        }

        putString(8, startY + i, to_string(processes[idx].pid), LIGHT_GRAY, BLACK);

        string name = processes[idx].name;
        if (name.length() > 25) name = name.substr(0, 22) + "...";

        if (idx == selectedProcess) {
            putString(12, startY + i, name, YELLOW, BLACK);
        }
        else {
            putString(12, startY + i, name, LIGHT_GRAY, BLACK);
        }

        putString(40, startY + i, to_string(processes[idx].memoryUsage), LIGHT_GRAY, BLACK);
        putString(60, startY + i, "[RUNNING]", LIGHT_GREEN, BLACK);
    }

    if (processes.size() > (size_t)visibleItems) {
        if (processScrollOffset > 0) {
            putChar(70, 8, '^', LIGHT_GRAY, BLACK);
        }
        else {
            putChar(70, 8, '^', DARK_GRAY, BLACK);
        }

        if (processScrollOffset + visibleItems < (int)processes.size()) {
            putChar(70, 8 + visibleItems, 'v', LIGHT_GRAY, BLACK);
        }
        else {
            putChar(70, 8 + visibleItems, 'v', DARK_GRAY, BLACK);
        }
    }

    drawBox(4, 28, CONSOLE_WIDTH - 44, 6, GREEN, "Process Controls", true);
    putString(6, 30, "Ctrl+R: Refresh", LIGHT_GREEN, BLACK);
    putString(35, 30, "Ctrl+K: Kill Process", LIGHT_RED, BLACK);
    putString(65, 30, "ESC: Back", YELLOW, BLACK);

    putString(6, 32, "Selected PID: ", YELLOW, BLACK);
    if (selectedProcess < (int)processes.size()) {
        putString(6 + 14, 32, to_string(processes[selectedProcess].pid), LIGHT_GRAY, BLACK);
    }

    putString(30, 32, "Total Memory: ", YELLOW, BLACK);
    long long totalMem = 0;
    for (const auto& proc : processes) totalMem += proc.memoryUsage;
    putString(30 + 14, 32, to_string(totalMem) + " KB", LIGHT_GRAY, BLACK);

    putString(4, 35, "Keys: ", YELLOW, BLACK);
    putString(4 + 6, 35, "ARROWS=Navigate  Ctrl+R=Refresh  Ctrl+K=Kill  ESC=Back", LIGHT_GRAY, BLACK);
}

bool handleProcessMonitorMode() {
    clearScreen();
    updateScreen();
    needRedraw = true;
    selectedProcess = 0;
    processScrollOffset = 0;
    loadProcesses();
    needRedraw = true;

    while (true) {
        getSystemInfo();

        if (clock() - lastBlink > 500) {
            blinkState = !blinkState;
            lastBlink = clock();
        }

        printProcessMonitor();
        updateScreen();

        if (_kbhit()) {
            int key = _getch();
            if (key == 0 || key == 224) {
                key = _getch();
                if (key == 72 && selectedProcess > 0) {
                    playMoveSound();
                    selectedProcess--;
                    if (selectedProcess < processScrollOffset) {
                        processScrollOffset = selectedProcess;
                    }
                }
                if (key == 80 && selectedProcess < (int)processes.size() - 1) {
                    playMoveSound();
                    selectedProcess++;
                    if (selectedProcess >= processScrollOffset + 15) {
                        processScrollOffset = selectedProcess - 14;
                    }
                }
                if (key == 73) {
                    playMoveSound();
                    selectedProcess = max(0, selectedProcess - 5);
                    if (selectedProcess < processScrollOffset) {
                        processScrollOffset = selectedProcess;
                    }
                }
                if (key == 81) {
                    playMoveSound();
                    selectedProcess = min((int)processes.size() - 1, selectedProcess + 5);
                    if (selectedProcess >= processScrollOffset + 15) {
                        processScrollOffset = selectedProcess - 14;
                    }
                }
            }
            else if ((key == 18) && (GetKeyState(VK_CONTROL) & 0x8000)) {
                playSelectSound();
                loadProcesses();
                selectedProcess = 0;
                processScrollOffset = 0;
                needRedraw = true;
            }
            else if ((key == 11) && (GetKeyState(VK_CONTROL) & 0x8000)) {
                playSelectSound();
                if (selectedProcess < (int)processes.size()) {
                    printStatusBar("Cannot kill processes in demo mode", LIGHT_RED);
                    _getch();
                }
            }
            else if (key == 27) {
                playCloseSound();
                currentScreen = DESKTOP;
                return true;
            }
            else if ((key == 8) && (GetKeyState(VK_CONTROL) & 0x8000)) {
                playSelectSound();
                currentScreen = HELP;
                return true;
            }
            else if ((key == 16) && (GetKeyState(VK_CONTROL) & 0x8000)) {
                playSelectSound();
                systemMonitorVisible = !systemMonitorVisible;
                needRedraw = true;
            }
        }
        Sleep(50);
    }
    return false;
}

void printPaintApp() {
    static int lastPaintCursorX = -1;
    static int lastPaintCursorY = -1;
    static int lastCurrentColorIndex = -1;
    static bool lastBlinkState = false;

    bool cursorChanged = (lastPaintCursorX != paintCursorX || lastPaintCursorY != paintCursorY);
    bool colorChanged = (lastCurrentColorIndex != currentColorIndex);
    bool blinkChanged = (lastBlinkState != blinkState);
    bool canvasChanged = false;

    for (int y = 0; y < 30 && !canvasChanged; y++) {
        for (int x = 0; x < 80; x++) {
            if (paintCanvas[y][x].active != paintCanvasPrev[y][x].active ||
                paintCanvas[y][x].color != paintCanvasPrev[y][x].color) {
                canvasChanged = true;
                break;
            }
        }
    }

    if (!needRedraw && !cursorChanged && !colorChanged && !blinkChanged && !canvasChanged) {
        return;
    }

    lastPaintCursorX = paintCursorX;
    lastPaintCursorY = paintCursorY;
    lastCurrentColorIndex = currentColorIndex;
    lastBlinkState = blinkState;

    if (canvasChanged) {
        paintCanvasPrev = paintCanvas;
    }

    drawBox(2, 4, CONSOLE_WIDTH - 40, 30, LIGHT_MAGENTA, "Paint", true);
    drawBox(5, 6, 82, 30, BLACK);

    for (int y = 0; y < 30 && y < paintCanvas.size(); y++) {
        for (int x = 0; x < 80 && x < paintCanvas[y].size(); x++) {
            if (paintCanvas[y][x].active) {
                putChar(6 + x, 7 + y, ' ', paintCanvas[y][x].color, paintCanvas[y][x].color);
            }
            else {
                putChar(6 + x, 7 + y, ' ', BLACK, BLACK);
            }
        }
    }

    if (paintCursorY >= 0 && paintCursorY < paintCanvas.size() &&
        paintCursorX >= 0 && paintCursorX < paintCanvas[paintCursorY].size()) {

        if (blinkState) {
            putChar(6 + paintCursorX, 7 + paintCursorY, ' ', paintColors[currentColorIndex], paintColors[currentColorIndex]);
        }
        else {
            if (paintCanvas[paintCursorY][paintCursorX].active) {
                putChar(6 + paintCursorX, 7 + paintCursorY, ' ',
                    paintCanvas[paintCursorY][paintCursorX].color,
                    paintCanvas[paintCursorY][paintCursorX].color);
            }
            else {
                putChar(6 + paintCursorX, 7 + paintCursorY, ' ', BLACK, BLACK);
            }
        }
    }

    putString(90, 6, "Paint Tools", YELLOW, BLACK);
    putString(90, 8, "Canvas: 80x30", LIGHT_GRAY, BLACK);
    putString(90, 10, "Colors (0-9, [/]):", YELLOW, BLACK);

    for (int i = 0; i < 10; i++) {
        if (i == currentColorIndex) {
            putChar(90, 12 + i, '>', LIGHT_GREEN, BLACK);
            putChar(91, 12 + i, ' ', LIGHT_GREEN, BLACK);
        }
        else {
            putChar(90, 12 + i, ' ', LIGHT_GRAY, BLACK);
            putChar(91, 12 + i, ' ', LIGHT_GRAY, BLACK);
        }
        putString(92, 12 + i, to_string(i) + ": " + colorNames[i], paintColors[i], BLACK);
    }

    putString(90, 23, "Controls:", YELLOW, BLACK);
    putString(90, 25, "ARROWS: Move cursor", LIGHT_GRAY, BLACK);
    putString(90, 26, "0-9: Select color", LIGHT_GRAY, BLACK);
    putString(90, 27, "[/]: Prev/Next color", LIGHT_GRAY, BLACK);
    putString(90, 28, "ENTER: Paint pixel", LIGHT_GRAY, BLACK);
    putString(90, 29, "SPACE: Clear pixel", LIGHT_GRAY, BLACK);
    putString(90, 30, "Ctrl+C: Clear canvas", LIGHT_GRAY, BLACK);
    putString(90, 31, "Ctrl+S: Save", LIGHT_GRAY, BLACK);
    putString(90, 32, "ESC: Back", LIGHT_GRAY, BLACK);

    putString(90, 34, "Cursor: ", YELLOW, BLACK);
    putString(90 + 8, 34, "X:" + to_string(paintCursorX) + " Y:" + to_string(paintCursorY), LIGHT_GRAY, BLACK);

    putString(105, 34, "Color: ", YELLOW, BLACK);
    putString(105 + 7, 34, colorNames[currentColorIndex], paintColors[currentColorIndex], BLACK);
}

bool savePaintDrawing(const string& filename, bool overwrite = false) {
    string actualFilename = filename;

    if (actualFilename.empty()) {
        printStatusBar("ERROR: Filename is empty!", LIGHT_RED);
        return false;
    }

    if (actualFilename.find(".draw") == string::npos) {
        actualFilename += ".draw";
    }

    char buffer[MAX_PATH];
    GetCurrentDirectoryA(MAX_PATH, buffer);
    string currentPath(buffer);

    string fullPath;

    if (currentPath.find("drawings") == string::npos) {
        string drawingsPath = "C:\\SOSIOC\\main\\drawings\\" + actualFilename;

        if (actualFilename.find(":\\") != string::npos || actualFilename.find("\\\\") == 0) {
            fullPath = actualFilename;
        }
        else {
            string tempPath = currentPath + "\\" + actualFilename;
            DWORD attr = GetFileAttributesA(tempPath.c_str());

            if (attr == INVALID_FILE_ATTRIBUTES || overwrite) {
                fullPath = tempPath;
            }
            else {
                fullPath = drawingsPath;
            }
        }
    }
    else {
        if (actualFilename.find(":\\") != string::npos || actualFilename.find("\\\\") == 0) {
            fullPath = actualFilename;
        }
        else {
            fullPath = currentPath + "\\" + actualFilename;
        }
    }

    if (!overwrite) {
        DWORD attr = GetFileAttributesA(fullPath.c_str());
        if (attr != INVALID_FILE_ATTRIBUTES && !(attr & FILE_ATTRIBUTE_DIRECTORY)) {
            printStatusBar("File exists! Use overwrite=true to replace.", LIGHT_RED);
            return false;
        }
    }

    ofstream file(fullPath);
    if (!file.is_open()) {
        printStatusBar("ERROR: Cannot save to: " + fullPath, LIGHT_RED);
        return false;
    }

    file << "PAINTFILE\n";
    file << "Version: 1.0\n";

    int savedPoints = 0;
    for (int y = 0; y < 30 && y < paintCanvas.size(); y++) {
        for (int x = 0; x < 80 && x < paintCanvas[y].size(); x++) {
            if (paintCanvas[y][x].active) {
                file << x << " " << y << " " << (int)paintCanvas[y][x].color << "\n";
                savedPoints++;
            }
        }
    }

    file << "ENDFILE\n";
    file.close();

    if (paintFileToLoad.empty()) {
        paintFileToLoad = fullPath;
    }

    printStatusBar("Saved " + to_string(savedPoints) + " points to: " + fullPath, LIGHT_GREEN);
    loadFileList();

    return savedPoints > 0;
}

bool loadPaintDrawing(const string& filename) {
    string fullPath = filename;

    if (fullPath.find(":\\") == string::npos && fullPath.find("\\\\") != 0) {
        char buffer[MAX_PATH];
        GetCurrentDirectoryA(MAX_PATH, buffer);
        string currentPath(buffer);

        string testPath1 = currentPath + "\\" + filename;
        string testPath2 = "C:\\SOSIOC\\main\\drawings\\" + filename;

        if (GetFileAttributesA(testPath1.c_str()) != INVALID_FILE_ATTRIBUTES) {
            fullPath = testPath1;
        }
        else if (GetFileAttributesA(testPath2.c_str()) != INVALID_FILE_ATTRIBUTES) {
            fullPath = testPath2;
        }
        else {
            fullPath = filename;
        }
    }

    printStatusBar("Loading: " + fullPath, LIGHT_CYAN);

    ifstream file(fullPath);
    if (!file.is_open()) {
        printStatusBar("ERROR: Cannot open file: " + fullPath, LIGHT_RED);
        return false;
    }

    string header;
    getline(file, header);
    if (header != "PAINTFILE") {
        file.close();
        printStatusBar("ERROR: Not a PAINTFILE: " + header, LIGHT_RED);
        return false;
    }

    initializePaintCanvas();

    int x, y, color;
    int loadedPoints = 0;
    string line;

    getline(file, line);
    if (line.find("Version:") != string::npos) {
    }
    else if (line != "ENDFILE") {
        stringstream ss(line);
        if (ss >> x >> y >> color) {
            if (x >= 0 && x < 80 && y >= 0 && y < 30) {
                if (y < paintCanvas.size() && x < paintCanvas[y].size()) {
                    paintCanvas[y][x].active = true;
                    paintCanvas[y][x].color = (ConsoleColor)color;
                    paintCanvas[y][x].ch = ' ';
                    loadedPoints++;
                }
            }
        }
    }

    while (getline(file, line)) {
        if (line == "ENDFILE") break;

        stringstream ss(line);
        if (ss >> x >> y >> color) {
            if (x >= 0 && x < 80 && y >= 0 && y < 30) {
                if (y < paintCanvas.size() && x < paintCanvas[y].size()) {
                    paintCanvas[y][x].active = true;
                    paintCanvas[y][x].color = (ConsoleColor)color;
                    paintCanvas[y][x].ch = ' ';
                    loadedPoints++;
                }
            }
        }
    }

    file.close();
    paintFileLoaded = true;
    paintFileToLoad = fullPath;

    printStatusBar("Loaded " + to_string(loadedPoints) + " points from: " + filename, LIGHT_GREEN);

    return loadedPoints > 0;
}

bool handlePaintMode() {
    clearScreen();
    updateScreen();
    needRedraw = true;
    if (!paintFileLoaded) {
        initializePaintCanvas();
    }
    paintFileLoaded = false;

    needRedraw = true;

    while (true) {
        getSystemInfo();

        if (clock() - lastBlink > 500) {
            blinkState = !blinkState;
            lastBlink = clock();
        }

        printPaintApp();
        updateScreen();

        if (_kbhit()) {
            int key = _getch();
            if (key == 0 || key == 224) {
                key = _getch();

                bool altPressed = (GetKeyState(VK_MENU) & 0x8000) != 0;

                if (key == 72) {
                    playMoveSound();
                    if (altPressed) {
                        currentColorIndex--;
                        if (currentColorIndex < 0) currentColorIndex = 9;
                        printStatusBar("Selected color: " + colorNames[currentColorIndex], paintColors[currentColorIndex]);
                    }
                    else {
                        if (paintCursorY > 0) {
                            paintCursorY--;
                        }
                    }
                }
                else if (key == 80) {
                    playMoveSound();
                    if (altPressed) {
                        currentColorIndex++;
                        if (currentColorIndex > 9) currentColorIndex = 0;
                        printStatusBar("Selected color: " + colorNames[currentColorIndex], paintColors[currentColorIndex]);
                    }
                    else {
                        if (paintCursorY < 29) {
                            paintCursorY++;
                        }
                    }
                }
                else if (key == 75 && paintCursorX > 0) {
                    playMoveSound();
                    paintCursorX--;
                }
                else if (key == 77 && paintCursorX < 79) {
                    playMoveSound();
                    paintCursorX++;
                }
            }
            else if (key == 13) {
                playPaintSound();
                if (paintCursorY >= 0 && paintCursorY < paintCanvas.size() &&
                    paintCursorX >= 0 && paintCursorX < paintCanvas[paintCursorY].size()) {
                    paintCanvas[paintCursorY][paintCursorX].active = true;
                    paintCanvas[paintCursorY][paintCursorX].color = paintColors[currentColorIndex];
                    paintCanvas[paintCursorY][paintCursorX].ch = ' ';
                }
            }
            else if (key == 32) {
                playMoveSound();
                if (paintCursorY >= 0 && paintCursorY < paintCanvas.size() &&
                    paintCursorX >= 0 && paintCursorX < paintCanvas[paintCursorY].size()) {
                    paintCanvas[paintCursorY][paintCursorX].active = false;
                }
            }
            else if (key >= '0' && key <= '9') {
                playMoveSound();
                currentColorIndex = key - '0';
                printStatusBar("Selected color: " + colorNames[currentColorIndex], paintColors[currentColorIndex]);
            }
            else if ((key == 3) && (GetKeyState(VK_CONTROL) & 0x8000)) {
                playSelectSound();
                initializePaintCanvas();
            }
            else if ((key == 19) && (GetKeyState(VK_CONTROL) & 0x8000)) {
                playSelectSound();
                markLineDirty(34);
                putString(6, 34, "Enter filename (without .draw): ", YELLOW, BLACK);
                updateScreen();

                string filename = "";
                char ch;
                while ((ch = _getch()) != 13) {
                    if (ch == 8) {
                        if (!filename.empty()) {
                            filename.pop_back();
                            putChar(6 + 31 + filename.length(), 34, ' ', YELLOW, BLACK);
                            setCursorPos(6 + 31 + filename.length(), 34);
                        }
                    }
                    else if (isprint(ch)) {
                        filename += ch;
                        putChar(6 + 31 + filename.length() - 1, 34, ch, YELLOW, BLACK);
                        setCursorPos(6 + 31 + filename.length(), 34);
                        updateScreen();
                    }
                }

                if (!filename.empty()) {
                    if (savePaintDrawing(filename, false)) {
                        playSuccessSound();
                        printStatusBar("Saved as: " + filename + ".draw", LIGHT_GREEN);
                    }
                    else {
                        playErrorSound();
                        printStatusBar("ERROR: Failed to save!", LIGHT_RED);
                    }
                }
                Sleep(1500);
                markLineDirty(34);
            }
            else if (key == 27) {
                playCloseSound();
                currentScreen = DESKTOP;
                return true;
            }
            else if ((key == 8) && (GetKeyState(VK_CONTROL) & 0x8000)) {
                playSelectSound();
                currentScreen = HELP;
                return true;
            }
            else if ((key == 16) && (GetKeyState(VK_CONTROL) & 0x8000)) {
                playSelectSound();
                systemMonitorVisible = !systemMonitorVisible;
                needRedraw = true;
            }
            else if (key == '[' || key == '{') {
                playMoveSound();
                currentColorIndex--;
                if (currentColorIndex < 0) currentColorIndex = 9;
                printStatusBar("Selected color: " + colorNames[currentColorIndex], paintColors[currentColorIndex]);
            }
            else if (key == ']' || key == '}') {
                playMoveSound();
                currentColorIndex++;
                if (currentColorIndex > 9) currentColorIndex = 0;
                printStatusBar("Selected color: " + colorNames[currentColorIndex], paintColors[currentColorIndex]);
            }
        }
        Sleep(50);
    }
    return false;
}

void printMusicPlayer() {
    static int lastSelectedMusic = -1;
    static int lastMusicScrollOffset = -1;
    static int lastCurrentSong = -2;
    static bool lastMusicPlaying = false;
    static int lastMusicProgress = -1;

    bool selectionChanged = (lastSelectedMusic != selectedMusic);
    bool scrollChanged = (lastMusicScrollOffset != musicScrollOffset);
    bool songChanged = (lastCurrentSong != currentSong);
    bool playingChanged = (lastMusicPlaying != musicPlaying);
    bool progressChanged = (lastMusicProgress != musicProgress);

    if (!needRedraw && !selectionChanged && !scrollChanged && !songChanged && !playingChanged && !progressChanged) {
        return;
    }

    lastSelectedMusic = selectedMusic;
    lastMusicScrollOffset = musicScrollOffset;
    lastCurrentSong = currentSong;
    lastMusicPlaying = musicPlaying;
    lastMusicProgress = musicProgress;

    drawBox(2, 4, CONSOLE_WIDTH - 40, 30, LIGHT_MAGENTA, "Music Player", true);

    putString(4, 6, "Playlist:", YELLOW, BLACK);
    drawBox(4, 7, CONSOLE_WIDTH - 44, 15, BLUE);

    putString(6, 8, "#", YELLOW, BLACK);
    putString(10, 8, "Title", YELLOW, BLACK);
    putString(40, 8, "Artist", YELLOW, BLACK);
    putString(60, 8, "Duration", YELLOW, BLACK);
    putString(75, 8, "Notes", YELLOW, BLACK);
    putString(85, 8, "Status", YELLOW, BLACK);

    for (int i = 0; i < CONSOLE_WIDTH - 50; i++) {
        putChar(6 + i, 9, char(196), LIGHT_GRAY, BLACK);
    }

    int startY = 10;
    int visibleItems = min(10, (int)playlist.size());
    for (int i = 0; i < visibleItems; i++) {
        int idx = i + musicScrollOffset;
        if (idx >= (int)playlist.size()) break;

        if (idx == selectedMusic) {
            putChar(6, startY + i, char(16), LIGHT_GREEN, BLACK);
        }
        else {
            putChar(6, startY + i, ' ', LIGHT_GRAY, BLACK);
        }

        putString(8, startY + i, to_string(idx + 1), LIGHT_GRAY, BLACK);

        string title = playlist[idx].title;
        if (title.length() > 25) title = title.substr(0, 22) + "...";

        if (idx == selectedMusic) {
            putString(10, startY + i, title, YELLOW, BLACK);
        }
        else if (idx == currentSong) {
            putString(10, startY + i, title, LIGHT_CYAN, BLACK);
        }
        else {
            putString(10, startY + i, title, LIGHT_GRAY, BLACK);
        }

        putString(40, startY + i, playlist[idx].artist, LIGHT_GRAY, BLACK);

        int minutes = playlist[idx].duration / 60;
        int seconds = playlist[idx].duration % 60;
        string duration = to_string(minutes) + ":" + (seconds < 10 ? "0" : "") + to_string(seconds);
        putString(60, startY + i, duration, LIGHT_GRAY, BLACK);

        putString(75, startY + i, to_string(playlist[idx].notes.size()) + " notes", LIGHT_GRAY, BLACK);

        if (idx == currentSong && musicPlaying) {
            putString(85, startY + i, "[PLAYING]", LIGHT_GREEN, BLACK);
        }
        else if (idx == currentSong) {
            putString(85, startY + i, "[PAUSED]", LIGHT_YELLOW, BLACK);
        }
        else {
            putString(85, startY + i, "[READY]", LIGHT_GRAY, BLACK);
        }
    }

    if (playlist.size() > (size_t)visibleItems) {
        if (musicScrollOffset > 0) {
            putChar(95, 10, '^', LIGHT_GREEN, BLACK);
        }
        else {
            putChar(95, 10, '^', DARK_GRAY, BLACK);
        }

        if (musicScrollOffset + visibleItems < (int)playlist.size()) {
            putChar(95, 10 + visibleItems - 1, 'v', LIGHT_GREEN, BLACK);
        }
        else {
            putChar(95, 10 + visibleItems - 1, 'v', DARK_GRAY, BLACK);
        }
    }

    drawBox(4, 23, CONSOLE_WIDTH - 44, 8, GREEN, "Now Playing", true);

    if (currentSong >= 0 && currentSong < (int)playlist.size()) {
        putString(6, 25, "Now Playing: ", YELLOW, BLACK);
        putString(6 + 13, 25, playlist[currentSong].title, LIGHT_CYAN, BLACK);

        putString(6, 26, "Notes: ", YELLOW, BLACK);
        putString(6 + 7, 26, to_string(playlist[currentSong].notes.size()) + " notes", LIGHT_GRAY, BLACK);

        putString(6, 27, "Progress: ", YELLOW, BLACK);
        putString(6 + 10, 27, "[", LIGHT_GRAY, BLACK);

        int totalWidth = 50;
        int progressWidth = (musicProgress * totalWidth) / 100;

        for (int i = 0; i < totalWidth; i++) {
            if (i < progressWidth) {
                putChar(6 + 11 + i, 27, '=', LIGHT_GREEN, BLACK);
            }
            else {
                putChar(6 + 11 + i, 27, ' ', LIGHT_GRAY, BLACK);
            }
        }

        putString(6 + 11 + totalWidth, 27, "] " + to_string(musicProgress) + "%", LIGHT_GRAY, BLACK);
    }
    else {
        putString(6, 25, "No song playing", LIGHT_GRAY, BLACK);
    }

    drawBox(4, 32, CONSOLE_WIDTH - 44, 3, CYAN, "Controls", true);
    putString(6, 33, "Ctrl+P: Play/Pause", LIGHT_GREEN, BLACK);
    putString(30, 33, "ENTER: Select", LIGHT_GREEN, BLACK);
    putString(60, 33, "Ctrl+N: Next", LIGHT_CYAN, BLACK);
    putString(85, 33, "ESC: Back", YELLOW, BLACK);

    putString(4, 35, "Keys: ", YELLOW, BLACK);
    putString(4 + 6, 35, "ARROWS=Navigate  Ctrl+P=Play/Pause  ENTER=Select  Ctrl+N=Next  ESC=Back", LIGHT_GRAY, BLACK);
}

bool handleMusicPlayerMode() {
    clearScreen();
    updateScreen();
    needRedraw = true;
    initializeMusicPlayer();
    needRedraw = true;

    while (true) {
        getSystemInfo();

        if (clock() - lastBlink > 500) {
            blinkState = !blinkState;
            lastBlink = clock();
        }

        if (musicPlaying && currentSong >= 0) {
            static clock_t lastProgressUpdate = clock();
            if (clock() - lastProgressUpdate > 1000) {
                musicProgress = min(100, musicProgress + 1);
                lastProgressUpdate = clock();
                if (musicProgress >= 100) {
                    musicPlaying = false;
                    musicProgress = 0;
                    currentSong = -1;
                }
            }
        }

        printMusicPlayer();
        updateScreen();

        if (_kbhit()) {
            int key = _getch();
            if (key == 0 || key == 224) {
                key = _getch();
                if (key == 72 && selectedMusic > 0) {
                    playMoveSound();
                    selectedMusic--;
                    if (selectedMusic < musicScrollOffset) {
                        musicScrollOffset = selectedMusic;
                    }
                }
                if (key == 80 && selectedMusic < (int)playlist.size() - 1) {
                    playMoveSound();
                    selectedMusic++;
                    if (selectedMusic >= musicScrollOffset + 10) {
                        musicScrollOffset = selectedMusic - 9;
                    }
                }
                if (key == 73) {
                    playMoveSound();
                    selectedMusic = max(0, selectedMusic - 5);
                    musicScrollOffset = max(0, musicScrollOffset - 5);
                }
                if (key == 81) {
                    playMoveSound();
                    selectedMusic = min((int)playlist.size() - 1, selectedMusic + 5);
                    musicScrollOffset = min(max(0, (int)playlist.size() - 10), musicScrollOffset + 5);
                }
            }
            else if ((key == 16) && (GetKeyState(VK_CONTROL) & 0x8000)) {
                playSelectSound();
                if (currentSong == selectedMusic) {
                    musicPlaying = !musicPlaying;
                }
                else {
                    currentSong = selectedMusic;
                    musicPlaying = true;
                    musicProgress = 0;
                    printStatusBar("Playing notes...", LIGHT_CYAN);
                    playMusicNotes(playlist[currentSong].notes);
                    printStatusBar("Music Player - Simple music player with 5-note melodies", LIGHT_GREEN);
                }
            }
            else if (key == 13) {
                playSelectSound();
                currentSong = selectedMusic;
                musicPlaying = true;
                musicProgress = 0;
                printStatusBar("Playing notes...", LIGHT_CYAN);
                playMusicNotes(playlist[currentSong].notes);
                printStatusBar("Music Player - Simple music player with 5-note melodies", LIGHT_GREEN);
            }
            else if ((key == 14) && (GetKeyState(VK_CONTROL) & 0x8000)) {
                playSelectSound();
                currentSong = (currentSong + 1) % (int)playlist.size();
                musicPlaying = true;
                musicProgress = 0;
                selectedMusic = currentSong;
                if (selectedMusic >= musicScrollOffset + 10) {
                    musicScrollOffset = selectedMusic - 9;
                }
                else if (selectedMusic < musicScrollOffset) {
                    musicScrollOffset = selectedMusic;
                }
                printStatusBar("Playing notes...", LIGHT_CYAN);
                playMusicNotes(playlist[currentSong].notes);
                printStatusBar("Music Player - Simple music player with 5-note melodies", LIGHT_GREEN);
            }
            else if (key == 27) {
                playCloseSound();
                musicPlaying = false;
                currentScreen = DESKTOP;
                return true;
            }
            else if ((key == 8) && (GetKeyState(VK_CONTROL) & 0x8000)) {
                playSelectSound();
                currentScreen = HELP;
                return true;
            }
            else if ((key == 16) && (GetKeyState(VK_CONTROL) & 0x8000)) {
                playSelectSound();
                systemMonitorVisible = !systemMonitorVisible;
                needRedraw = true;
            }
        }
        Sleep(50);
    }
    return false;
}

void printMusicEditor() {
    static int lastMusicEditorCursor = -1;
    static bool lastBlinkState = false;

    bool cursorChanged = (lastMusicEditorCursor != musicEditorCursor);
    bool blinkChanged = (lastBlinkState != blinkState);

    if (!needRedraw && !cursorChanged && !blinkChanged) {
        return;
    }

    lastMusicEditorCursor = musicEditorCursor;
    lastBlinkState = blinkState;

    drawBox(2, 4, CONSOLE_WIDTH - 40, 30, LIGHT_YELLOW, "Music Editor", true);

    putString(4, 6, "Composition: ", YELLOW, BLACK);
    putString(4 + 13, 6, currentMusic.name, LIGHT_GREEN, BLACK);

    drawBox(4, 7, CONSOLE_WIDTH - 44, 20, BLACK);

    int visibleLines = min(18, (int)musicEditorLines.size());
    for (int i = 0; i < visibleLines; i++) {
        int lineIdx = i;
        if (musicEditorCursor >= visibleLines) {
            lineIdx = musicEditorCursor - visibleLines + i + 1;
            if (lineIdx >= (int)musicEditorLines.size()) lineIdx = (int)musicEditorLines.size() - 1;
        }

        if (lineIdx == musicEditorCursor) {
            putChar(6, 9 + i, '>', LIGHT_GREEN, BLACK);
        }
        else {
            putChar(6, 9 + i, ' ', LIGHT_GRAY, BLACK);
        }

        string lineNum = to_string(lineIdx + 1);
        if (lineNum.length() < 3) lineNum = string(3 - lineNum.length(), ' ') + lineNum;
        putString(7, 9 + i, lineNum + " | ", LIGHT_GRAY, BLACK);

        string displayLine = musicEditorLines[lineIdx];
        if (lineIdx == musicEditorCursor) {
            if (blinkState) {
                if (displayLine.length() > 0) {
                    displayLine += "_";
                }
                else {
                    displayLine = "_";
                }
            }
            if (displayLine.length() > CONSOLE_WIDTH - 53) {
                displayLine = displayLine.substr(0, CONSOLE_WIDTH - 53);
            }
            putString(11, 9 + i, displayLine, YELLOW, BLACK);
        }
        else {
            if (displayLine.length() > CONSOLE_WIDTH - 53) {
                displayLine = displayLine.substr(0, CONSOLE_WIDTH - 53) + "...";
            }
            if (displayLine.find("note(") != string::npos) {
                putString(11, 9 + i, displayLine, LIGHT_CYAN, BLACK);
            }
            else if (displayLine.find("//") != string::npos) {
                putString(11, 9 + i, displayLine, LIGHT_GRAY, BLACK);
            }
            else {
                putString(11, 9 + i, displayLine, LIGHT_GRAY, BLACK);
            }
        }
    }

    drawBox(4, 28, CONSOLE_WIDTH - 44, 6, CYAN, "Editor Commands", true);
    putString(6, 30, "CTRL+P: Play/Stop", LIGHT_GREEN, BLACK);
    putString(30, 30, "CTRL+S: Save", LIGHT_GREEN, BLACK);
    putString(60, 30, "CTRL+L: Load", LIGHT_GREEN, BLACK);
    putString(90, 30, "ESC: Back", LIGHT_RED, BLACK);

    putString(6, 31, "Notes: ", YELLOW, BLACK);
    putString(6 + 7, 31, to_string(currentMusic.notes.size()), LIGHT_GRAY, BLACK);

    putString(30, 31, "Status: ", YELLOW, BLACK);
    if (currentMusic.isPlaying) {
        putString(30 + 8, 31, "Playing...", LIGHT_GREEN, BLACK);
    }
    else {
        putString(30 + 8, 31, "Ready", LIGHT_GRAY, BLACK);
    }

    putString(60, 31, "Format: ", YELLOW, BLACK);
    putString(60 + 8, 31, "note(pitch, duration, delay)", LIGHT_GRAY, BLACK);

    putString(4, 35, "Keys: ", YELLOW, BLACK);
    putString(4 + 6, 35, "ARROWS=Navigate  ENTER=Edit  CTRL+P=Play  CTRL+S=Save  ESC=Back", LIGHT_GRAY, BLACK);
}

bool saveMusicComposition(const string& filename) {
    string fullPath = filename;
    if (fullPath.find("C:\\") == string::npos) {
        fullPath = "C:\\VirtualDisk\\Music\\" + filename;
    }

    ofstream file(fullPath);
    if (file.is_open()) {
        file << "SOSIOC_MUSIC_v1.0\n";
        file << "Name: " << currentMusic.name << "\n";
        file << "Notes:\n";
        for (const auto& line : musicEditorLines) {
            file << line << "\n";
        }
        file.close();
        return true;
    }
    return false;
}

bool loadMusicComposition(const string& filename) {
    string fullPath = filename;
    if (fullPath.find("C:\\") == string::npos) {
        fullPath = "C:\\VirtualDisk\\Music\\" + filename;
    }

    ifstream file(fullPath);
    if (!file.is_open()) {
        return false;
    }

    musicEditorLines.clear();
    currentMusic.notes.clear();
    string line;
    getline(file, line);
    getline(file, line);
    if (line.find("Name: ") != string::npos) {
        currentMusic.name = line.substr(6);
    }
    getline(file, line);

    while (getline(file, line)) {
        musicEditorLines.push_back(line);
        if (line.find("note(") != string::npos) {
            size_t start = line.find("(") + 1;
            size_t end = line.find(")");
            if (start != string::npos && end != string::npos) {
                string params = line.substr(start, end - start);
                stringstream ss(params);
                int pitch, duration, delay;
                char comma;
                if (ss >> pitch >> comma >> duration >> comma >> delay) {
                    MusicNote note;
                    note.pitch = pitch;
                    note.duration = duration;
                    note.delay = delay;
                    currentMusic.notes.push_back(note);
                }
            }
        }
    }
    file.close();
    musicEditorCursor = 0;
    return true;
}

bool handleMusicEditorMode() {
    clearScreen();
    updateScreen();
    needRedraw = true;
    initializeMusicEditor();
    needRedraw = true;

    while (true) {
        getSystemInfo();

        if (clock() - lastBlink > 500) {
            blinkState = !blinkState;
            lastBlink = clock();
        }

        printMusicEditor();
        updateScreen();

        if (_kbhit()) {
            int key = _getch();
            if (key == 0 || key == 224) {
                key = _getch();
                if (key == 72 && musicEditorCursor > 0) {
                    playMoveSound();
                    musicEditorCursor--;
                }
                if (key == 80 && musicEditorCursor < (int)musicEditorLines.size() - 1) {
                    playMoveSound();
                    musicEditorCursor++;
                }
            }
            else if ((key == 16) && (GetKeyState(VK_CONTROL) & 0x8000)) {
                playSelectSound();
                if (!currentMusic.isPlaying) {
                    currentMusic.isPlaying = true;
                    currentMusic.currentNote = 0;
                    printStatusBar("Playing composition...", LIGHT_CYAN);
                    playMusicComposition();
                    currentMusic.isPlaying = false;
                    printStatusBar("Music Editor - Create music compositions", LIGHT_GREEN);
                }
                else {
                    currentMusic.isPlaying = false;
                }
            }
            else if ((key == 19) && (GetKeyState(VK_CONTROL) & 0x8000)) {
                playSelectSound();
                static int saveCounter = 1;
                string filename = "composition_" + to_string(saveCounter) + ".music";
                if (saveMusicComposition(filename)) {
                    playSuccessSound();
                    printStatusBar("Composition saved to: C:\\VirtualDisk\\Music\\" + filename, LIGHT_GREEN);
                    saveCounter++;
                }
                else {
                    playErrorSound();
                    printStatusBar("ERROR: Failed to save composition", LIGHT_RED);
                }
                _getch();
            }
            else if ((key == 12) && (GetKeyState(VK_CONTROL) & 0x8000)) {
                playSelectSound();
                markLineDirty(34);
                putString(6, 34, "Enter filename to load: ", LIGHT_GREEN, BLACK);
                updateScreen();

                string filename;
                char ch;
                while ((ch = _getch()) != 13) {
                    if (ch == 8 && !filename.empty()) {
                        filename.pop_back();
                    }
                    else if (isprint(ch)) {
                        filename += ch;
                    }
                }
                if (loadMusicComposition(filename)) {
                    playSuccessSound();
                    printStatusBar("Composition loaded: " + filename, LIGHT_GREEN);
                }
                else {
                    playErrorSound();
                    printStatusBar("ERROR: Failed to load composition", LIGHT_RED);
                }
                _getch();
                markLineDirty(34);
            }
            else if (key == 13) {
                playSelectSound();
                markLineDirty(34);
                putString(6, 34, "Enter note in format: note(pitch, duration, delay) // comment", LIGHT_CYAN, BLACK);
                updateScreen();

                string newLine = "";
                char ch;
                while ((ch = _getch()) != 13) {
                    if (ch == 8 && !newLine.empty()) {
                        newLine.pop_back();
                    }
                    else if (isprint(ch)) {
                        newLine += ch;
                    }
                }
                if (!newLine.empty()) {
                    musicEditorLines.insert(musicEditorLines.begin() + musicEditorCursor + 1, newLine);
                    musicEditorCursor++;
                    if (newLine.find("note(") != string::npos) {
                        size_t start = newLine.find("(") + 1;
                        size_t end = newLine.find(")");
                        if (start != string::npos && end != string::npos) {
                            string params = newLine.substr(start, end - start);
                            stringstream ss(params);
                            int pitch, duration, delay;
                            char comma;
                            if (ss >> pitch >> comma >> duration >> comma >> delay) {
                                MusicNote note;
                                note.pitch = pitch;
                                note.duration = duration;
                                note.delay = delay;
                                currentMusic.notes.push_back(note);
                            }
                        }
                    }
                }
                printStatusBar("Music Editor - Create music compositions", LIGHT_GREEN);
                markLineDirty(34);
            }
            else if (key == 27) {
                playCloseSound();
                currentMusic.isPlaying = false;
                currentScreen = DESKTOP;
                return true;
            }
            else if ((key == 8) && (GetKeyState(VK_CONTROL) & 0x8000)) {
                playSelectSound();
                currentScreen = HELP;
                return true;
            }
            else if ((key == 16) && (GetKeyState(VK_CONTROL) & 0x8000)) {
                playSelectSound();
                systemMonitorVisible = !systemMonitorVisible;
                needRedraw = true;
            }
        }
        Sleep(50);
    }
    return false;
}

void printWeatherApp() {
    static int lastWeatherDay = -1;
    static string lastCondition = "";
    static double lastTemperature = -1000;

    bool dayChanged = (lastWeatherDay != weatherDay);
    bool conditionChanged = (lastCondition != currentWeather.condition);
    bool tempChanged = (abs(lastTemperature - currentWeather.temperature) > 0.1);

    if (!needRedraw && !dayChanged && !conditionChanged && !tempChanged) {
        return;
    }

    lastWeatherDay = weatherDay;
    lastCondition = currentWeather.condition;
    lastTemperature = currentWeather.temperature;

    drawBox(2, 4, CONSOLE_WIDTH - 40, 30, LIGHT_CYAN, "Weather Forecast", true);

    putString(4, 6, "Current Weather:", YELLOW, BLACK);
    drawBox(4, 7, CONSOLE_WIDTH - 44, 10, BLUE);

    putString(6, 8, "City: ", YELLOW, BLACK);
    putString(6 + 6, 8, currentWeather.city, LIGHT_GREEN, BLACK);

    putString(6, 9, "Temperature: ", YELLOW, BLACK);
    string tempStr = to_string(currentWeather.temperature).substr(0, 4) + "°C";
    if (currentWeather.temperature > 25) {
        putString(6 + 13, 9, tempStr, LIGHT_RED, BLACK);
    }
    else if (currentWeather.temperature > 15) {
        putString(6 + 13, 9, tempStr, YELLOW, BLACK);
    }
    else {
        putString(6 + 13, 9, tempStr, LIGHT_BLUE, BLACK);
    }

    putString(6, 10, "Condition: ", YELLOW, BLACK);
    ConsoleColor condColor = LIGHT_GRAY;
    if (currentWeather.condition == "Sunny") condColor = YELLOW;
    else if (currentWeather.condition == "Cloudy") condColor = LIGHT_GRAY;
    else if (currentWeather.condition == "Rainy") condColor = LIGHT_BLUE;
    else if (currentWeather.condition == "Windy") condColor = LIGHT_CYAN;
    putString(6 + 11, 10, currentWeather.condition, condColor, BLACK);

    putString(6, 11, "Humidity: ", YELLOW, BLACK);
    putString(6 + 10, 11, to_string(currentWeather.humidity).substr(0, 4) + "%", LIGHT_GRAY, BLACK);

    putString(6, 12, "Wind Speed: ", YELLOW, BLACK);
    putString(6 + 12, 12, to_string(currentWeather.windSpeed).substr(0, 4) + " km/h", LIGHT_GRAY, BLACK);

    putString(6, 13, "Forecast: ", YELLOW, BLACK);
    putString(6 + 10, 13, currentWeather.forecast, LIGHT_GREEN, BLACK);

    drawBox(4, 18, CONSOLE_WIDTH - 44, 10, GREEN, "7-Day Forecast", true);

    string days[] = { "Mon", "Tue", "Wed", "Thu", "Fri", "Sat", "Sun" };
    string conditions[] = { "Sunny", "Cloudy", "Rainy", "Windy", "Sunny", "Cloudy", "Rainy" };

    for (int i = 0; i < 7; i++) {
        int dayX = 10 + i * 15;

        if (i == weatherDay) {
            putString(dayX, 20, "[" + days[i] + "]", LIGHT_GREEN, BLACK);
        }
        else {
            putString(dayX, 20, " " + days[i] + " ", LIGHT_GRAY, BLACK);
        }

        double temp = 10.0 + 15.0 * sin((i + weatherDay) * 0.5);
        string tempDayStr = to_string(temp).substr(0, 4) + "°C";

        if (temp > 25) {
            putString(dayX, 22, tempDayStr, LIGHT_RED, BLACK);
        }
        else if (temp > 15) {
            putString(dayX, 22, tempDayStr, YELLOW, BLACK);
        }
        else {
            putString(dayX, 22, tempDayStr, LIGHT_BLUE, BLACK);
        }

        ConsoleColor dayCondColor = LIGHT_GRAY;
        if (conditions[i] == "Sunny") dayCondColor = YELLOW;
        else if (conditions[i] == "Cloudy") dayCondColor = LIGHT_GRAY;
        else if (conditions[i] == "Rainy") dayCondColor = LIGHT_BLUE;
        else if (conditions[i] == "Windy") dayCondColor = LIGHT_CYAN;

        putString(dayX, 23, conditions[i], dayCondColor, BLACK);

        int rainChance = (i * 15) % 100;
        if (conditions[i] == "Rainy") {
            putString(dayX, 24, to_string(rainChance) + "% rain", LIGHT_BLUE, BLACK);
        }
        else {
            putString(dayX, 24, to_string(rainChance) + "% rain", LIGHT_GRAY, BLACK);
        }
    }

    drawBox(4, 29, CONSOLE_WIDTH - 44, 4, MAGENTA, "Weather Controls", true);
    putString(6, 31, "Ctrl+R: Refresh Weather", LIGHT_GREEN, BLACK);
    putString(40, 31, "LEFT/RIGHT: Change Day", LIGHT_CYAN, BLACK);
    putString(80, 31, "ESC: Back", YELLOW, BLACK);

    putString(4, 35, "Keys: ", YELLOW, BLACK);
    putString(4 + 6, 35, "Ctrl+R=Refresh  LEFT/RIGHT=Change day  ESC=Back", LIGHT_GRAY, BLACK);
}

bool handleWeatherMode() {
    clearScreen();
    updateScreen();
    needRedraw = true;
    initializeWeather();
    needRedraw = true;

    while (true) {
        getSystemInfo();
        updateWeather();

        printWeatherApp();
        updateScreen();

        if (_kbhit()) {
            int key = _getch();
            if (key == 0 || key == 224) {
                key = _getch();
                if (key == 75) {
                    playMoveSound();
                    weatherDay = (weatherDay - 1 + 7) % 7;
                }
                if (key == 77) {
                    playMoveSound();
                    weatherDay = (weatherDay + 1) % 7;
                }
            }
            else if ((key == 18) && (GetKeyState(VK_CONTROL) & 0x8000)) {
                playSelectSound();
                updateWeather();
                printStatusBar("Weather data refreshed", LIGHT_GREEN);
            }
            else if (key == 27) {
                playCloseSound();
                currentScreen = DESKTOP;
                return true;
            }
            else if ((key == 8) && (GetKeyState(VK_CONTROL) & 0x8000)) {
                playSelectSound();
                currentScreen = HELP;
                return true;
            }
            else if ((key == 16) && (GetKeyState(VK_CONTROL) & 0x8000)) {
                playSelectSound();
                systemMonitorVisible = !systemMonitorVisible;
                needRedraw = true;
            }
        }
        Sleep(100);
    }
    return false;
}

void printArcadeGame() {
    static int lastScore = -1;
    static int lastHighScore = -1;
    static size_t lastSnakeSize = 0;
    static int lastFoodX = -1;
    static int lastFoodY = -1;
    static bool lastGameOver = false;
    static bool lastGamePaused = false;
    static bool lastBlinkState = false;

    bool scoreChanged = (lastScore != score || lastHighScore != highScore);
    bool snakeChanged = (lastSnakeSize != snake.size());
    bool foodChanged = (lastFoodX != foodX || lastFoodY != foodY);
    bool gameStateChanged = (lastGameOver != gameOver || lastGamePaused != gamePaused);
    bool blinkChanged = (lastBlinkState != blinkState);

    if (!needRedraw && !scoreChanged && !snakeChanged && !foodChanged && !gameStateChanged && !blinkChanged) {
        return;
    }

    lastScore = score;
    lastHighScore = highScore;
    lastSnakeSize = snake.size();
    lastFoodX = foodX;
    lastFoodY = foodY;
    lastGameOver = gameOver;
    lastGamePaused = gamePaused;
    lastBlinkState = blinkState;

    drawBox(2, 4, CONSOLE_WIDTH - 40, 30, LIGHT_RED, "Arcade Game - Snake", true);
    drawBox(5, 6, 75, 30, BLACK);

    for (int y = 5; y < 35; y++) {
        for (int x = 2; x < 77; x++) {
            if (x == 2 || x == 76 || y == 5 || y == 34) {
                putChar(x, y, '#', LIGHT_GRAY, BLACK);
            }
            else {
                putChar(x, y, ' ', LIGHT_GRAY, BLACK);
            }
        }
    }

    for (size_t i = 0; i < snake.size(); i++) {
        if (i == 0) {
            if (!gameOver && !gamePaused && blinkState) {
                putChar(snake[i].x, snake[i].y, '@', YELLOW, BLACK);
            }
            else {
                putChar(snake[i].x, snake[i].y, '@', LIGHT_GREEN, BLACK);
            }
        }
        else {
            putChar(snake[i].x, snake[i].y, 'O', LIGHT_CYAN, BLACK);
        }
    }

    putChar(foodX, foodY, '*', LIGHT_RED, BLACK);

    drawBox(85, 6, 30, 25, BLUE, "Game Info", true);
    putString(87, 8, "Score: ", YELLOW, BLACK);
    putString(87 + 7, 8, to_string(score), LIGHT_GRAY, BLACK);

    putString(87, 10, "High Score: ", YELLOW, BLACK);
    putString(87 + 12, 10, to_string(highScore), LIGHT_GRAY, BLACK);

    putString(87, 12, "Length: ", YELLOW, BLACK);
    putString(87 + 8, 12, to_string(snake.size()), LIGHT_GRAY, BLACK);

    putString(87, 14, "Speed: ", YELLOW, BLACK);
    putString(87 + 7, 14, to_string((250 - gameSpeed) / 25 + 1), LIGHT_GRAY, BLACK);

    drawBox(85, 18, 30, 8, GREEN, "Controls", true);
    putString(87, 20, "ARROWS: Move", LIGHT_GREEN, BLACK);
    putString(87, 21, "Ctrl+P: Pause/Resume", LIGHT_GREEN, BLACK);
    putString(87, 22, "Ctrl+R: Restart", LIGHT_CYAN, BLACK);
    putString(87, 23, "ESC: Back", LIGHT_RED, BLACK);

    drawBox(85, 27, 30, 6, MAGENTA, "Game Status", true);
    putString(87, 29, "Status: ", YELLOW, BLACK);

    if (gameOver) {
        putString(87 + 8, 29, "GAME OVER!", LIGHT_RED, BLACK);
    }
    else if (gamePaused) {
        putString(87 + 8, 29, "PAUSED", LIGHT_YELLOW, BLACK);
    }
    else {
        putString(87 + 8, 29, "PLAYING", LIGHT_GREEN, BLACK);
    }

    putString(4, 35, "Keys: ", YELLOW, BLACK);
    putString(4 + 6, 35, "ARROWS=Move  Ctrl+P=Pause  Ctrl+R=Restart  ESC=Back", LIGHT_GRAY, BLACK);
}

bool handleArcadeGameMode() {
    clearScreen();
    updateScreen();
    needRedraw = true;

    initializeArcadeGame();
    needRedraw = true;

    while (true) {
        getSystemInfo();

        if (clock() - lastBlink > 500) {
            blinkState = !blinkState;
            lastBlink = clock();
        }

        if (!gameOver && !gamePaused) {
            updateSnake();
        }

        printArcadeGame();
        updateScreen();

        if (_kbhit()) {
            int key = _getch();
            if (key == 0 || key == 224) {
                key = _getch();
                if (!gameOver && !gamePaused) {
                    playMoveSound();
                    switch (key) {
                    case 72: if (snakeDirection != DOWN) snakeDirection = UP; break;
                    case 80: if (snakeDirection != UP) snakeDirection = DOWN; break;
                    case 75: if (snakeDirection != RIGHT) snakeDirection = LEFT; break;
                    case 77: if (snakeDirection != LEFT) snakeDirection = RIGHT; break;
                    }
                }
            }
            else if ((key == 16) && (GetKeyState(VK_CONTROL) & 0x8000)) {
                playSelectSound();
                gamePaused = !gamePaused;
            }
            else if ((key == 18) && (GetKeyState(VK_CONTROL) & 0x8000)) {
                playSelectSound();
                initializeArcadeGame();
                needRedraw = true;
            }
            else if (key == 27) {
                playCloseSound();
                currentScreen = DESKTOP;
                return true;
            }
            else if ((key == 8) && (GetKeyState(VK_CONTROL) & 0x8000)) {
                playSelectSound();
                currentScreen = HELP;
                return true;
            }
            else if ((key == 16) && (GetKeyState(VK_CONTROL) & 0x8000)) {
                playSelectSound();
                systemMonitorVisible = !systemMonitorVisible;
                needRedraw = true;
            }
        }
        Sleep(gameSpeed);
    }
    return false;
}

void printRacingGame() {
    static int lastRaceScore = -1;
    static int lastGameDistance = -1;
    static int lastRaceLevel = -1;
    static int lastRaceHighScore = -1;
    static bool lastRaceOver = false;

    bool scoreChanged = (lastRaceScore != raceScore || lastGameDistance != gameDistance);
    bool levelChanged = (lastRaceLevel != raceLevel);
    bool highScoreChanged = (lastRaceHighScore != raceHighScore);
    bool raceOverChanged = (lastRaceOver != raceOver);

    if (!needRedraw && !scoreChanged && !levelChanged && !highScoreChanged && !raceOverChanged) {
        drawRoad();
        return;
    }

    lastRaceScore = raceScore;
    lastGameDistance = gameDistance;
    lastRaceLevel = raceLevel;
    lastRaceHighScore = raceHighScore;
    lastRaceOver = raceOver;

    drawBox(2, 4, CONSOLE_WIDTH - 40, 30, LIGHT_MAGENTA, "Racing Game", true);

    drawRoad();

    for (const auto& obs : obstacles) {
        if (obs.active && obs.y >= 5 && obs.y < 35) {
            for (int i = 0; i < 3; i++) {
                for (int j = 0; j < 3; j++) {
                    if (obs.y + i >= 5 && obs.y + i < 35 && obs.x + j >= roadPosition && obs.x + j < roadPosition + roadWidth) {
                        putChar(obs.x + j, obs.y + i, obs.shape[i][j],
                            obs.type == 0 ? RED : (obs.type == 1 ? YELLOW : LIGHT_GRAY), BLACK);
                    }
                }
            }
        }
    }

    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 6; j++) {
            if (playerCar.y + i >= 5 && playerCar.y + i < 35 && playerCar.x + j >= roadPosition && playerCar.x + j < roadPosition + roadWidth) {
                if (playerCar.crashed) {
                    putChar(playerCar.x + j, playerCar.y + i, 'X', RED, BLACK);
                }
                else {
                    putChar(playerCar.x + j, playerCar.y + i, playerCar.shape[i][j], (ConsoleColor)playerCar.color, BLACK);
                }
            }
        }
    }

    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 6; j++) {
            if (opponentCarY + i >= 5 && opponentCarY + i < 35 && opponentCarX + j >= roadPosition && opponentCarX + j < roadPosition + roadWidth) {
                putChar(opponentCarX + j, opponentCarY + i, playerCar.shape[i][j], LIGHT_RED, BLACK);
            }
        }
    }

    drawBox(85, 6, 30, 25, BLUE, "Game Stats", true);
    putString(87, 8, "Score: ", YELLOW, BLACK);
    putString(87 + 7, 8, to_string(raceScore), LIGHT_GRAY, BLACK);

    putString(87, 10, "Distance: ", YELLOW, BLACK);
    putString(87 + 10, 10, to_string(gameDistance), LIGHT_GRAY, BLACK);

    putString(87, 12, "Level: ", YELLOW, BLACK);
    putString(87 + 7, 12, to_string(raceLevel), LIGHT_GRAY, BLACK);

    putString(87, 14, "Speed: ", YELLOW, BLACK);
    putString(87 + 7, 14, to_string((150 - raceSpeed) / 10 + 1), LIGHT_GRAY, BLACK);

    putString(87, 16, "High Score: ", YELLOW, BLACK);
    putString(87 + 12, 16, to_string(raceHighScore), LIGHT_GRAY, BLACK);

    putString(87, 18, "Obstacles: ", YELLOW, BLACK);
    putString(87 + 11, 18, to_string(obstacles.size()), LIGHT_GRAY, BLACK);

    drawBox(85, 22, 30, 8, GREEN, "Controls", true);
    putString(87, 24, "LEFT/RIGHT: Steer", LIGHT_GREEN, BLACK);
    putString(87, 25, "UP/DOWN: Speed", LIGHT_GREEN, BLACK);
    putString(87, 26, "SPACE: Boost", LIGHT_CYAN, BLACK);
    putString(87, 27, "R: Restart", LIGHT_CYAN, BLACK);
    putString(87, 28, "ESC: Back", LIGHT_RED, BLACK);

    drawBox(85, 31, 30, 4, MAGENTA, "Status", true);
    putString(87, 32, "Status: ", YELLOW, BLACK);

    if (raceOver) {
        putString(87 + 8, 32, "CRASHED!", LIGHT_RED, BLACK);
    }
    else {
        putString(87 + 8, 32, "RACING", LIGHT_GREEN, BLACK);
    }

    putString(4, 35, "Keys: ", YELLOW, BLACK);
    putString(4 + 6, 35, "LEFT/RIGHT=Steer  UP/DOWN=Speed  SPACE=Boost  R=Restart  ESC=Back", LIGHT_GRAY, BLACK);
}

bool handleRacingGameMode() {
    clearScreen();
    updateScreen();
    needRedraw = true;
    initializeRacingGame();
    needRedraw = true;

    while (true) {
        getSystemInfo();

        if (clock() - lastBlink > 500) {
            blinkState = !blinkState;
            lastBlink = clock();
        }

        if (!raceOver) {
            updateRacingGame();
        }

        printRacingGame();
        updateScreen();

        if (_kbhit()) {
            int key = _getch();
            if (key == 0 || key == 224) {
                key = _getch();
                if (!raceOver) {
                    if (key == 72) {
                        playMoveSound();
                        if (raceSpeed > 30) raceSpeed -= 10;
                    }
                    if (key == 80) {
                        playMoveSound();
                        if (raceSpeed < 150) raceSpeed += 10;
                    }
                    if (key == 75) {
                        playMoveSound();
                        if (playerCar.x > roadPosition + 2) {
                            playerCar.x -= 2;
                        }
                    }
                    if (key == 77) {
                        playMoveSound();
                        if (playerCar.x < roadPosition + roadWidth - 8) {
                            playerCar.x += 2;
                        }
                    }
                }
            }
            else if (key == 32 && !raceOver) {
                playBoostSound();
                raceSpeed = max(20, raceSpeed - 20);
                raceScore += 10;
            }
            else if (key == 'r' || key == 'R') {
                playSelectSound();
                initializeRacingGame();
                needRedraw = true;
            }
            else if (key == 27) {
                playCloseSound();
                currentScreen = DESKTOP;
                return true;
            }
            else if ((key == 8) && (GetKeyState(VK_CONTROL) & 0x8000)) {
                playSelectSound();
                currentScreen = HELP;
                return true;
            }
            else if ((key == 16) && (GetKeyState(VK_CONTROL) & 0x8000)) {
                playSelectSound();
                systemMonitorVisible = !systemMonitorVisible;
                needRedraw = true;
            }
        }

        Sleep(raceSpeed);
    }
    return false;
}

void printClockApp() {
    static bool lastShowAnalog = false;
    static string lastTimeStr = "";
    static bool lastAlarmSet = false;
    static bool lastTimerRunning = false;
    static int lastTimerSeconds = -1;

    time_t now = time(0);
    tm* ltm = localtime(&now);
    char timeStr[9];
    strftime(timeStr, sizeof(timeStr), "%H:%M:%S", ltm);

    bool analogChanged = (lastShowAnalog != clockData.showAnalog);
    bool timeChanged = (lastTimeStr != string(timeStr));
    bool alarmChanged = (lastAlarmSet != clockData.alarmSet);
    bool timerChanged = (lastTimerRunning != clockData.timerRunning ||
        lastTimerSeconds != clockData.timerSeconds);

    if (!needRedraw && !analogChanged && !timeChanged && !alarmChanged && !timerChanged) {
        return;
    }

    lastShowAnalog = clockData.showAnalog;
    lastTimeStr = timeStr;
    lastAlarmSet = clockData.alarmSet;
    lastTimerRunning = clockData.timerRunning;
    lastTimerSeconds = clockData.timerSeconds;

    drawBox(2, 4, CONSOLE_WIDTH - 40, 30, LIGHT_BLUE, "Digital Clock", true);

    putString(4, 6, "Current Time:", YELLOW, BLACK);
    drawBox(4, 7, CONSOLE_WIDTH - 44, 10, CYAN);

    if (clockData.showAnalog) {
        int centerX = 60;
        int centerY = 15;
        int radius = 10;

        for (int y = centerY - radius; y <= centerY + radius; y++) {
            for (int x = centerX - radius; x <= centerX + radius; x++) {
                double dist = sqrt(pow(x - centerX, 2) + pow(y - centerY, 2));
                if (abs(dist - radius) < 0.5) {
                    putChar(x, y, 'O', LIGHT_CYAN, BLACK);
                }
                else {
                    putChar(x, y, ' ', LIGHT_GRAY, BLACK);
                }
            }
        }

        int hourAngle = (ltm->tm_hour % 12) * 30 + ltm->tm_min / 2;
        int minuteAngle = ltm->tm_min * 6;
        int secondAngle = ltm->tm_sec * 6;

        int hourX = centerX + (int)(radius * 0.5 * sin(hourAngle * 3.14159 / 180));
        int hourY = centerY - (int)(radius * 0.5 * cos(hourAngle * 3.14159 / 180));
        int minuteX = centerX + (int)(radius * 0.8 * sin(minuteAngle * 3.14159 / 180));
        int minuteY = centerY - (int)(radius * 0.8 * cos(minuteAngle * 3.14159 / 180));
        int secondX = centerX + (int)(radius * 0.9 * sin(secondAngle * 3.14159 / 180));
        int secondY = centerY - (int)(radius * 0.9 * cos(secondAngle * 3.14159 / 180));

        for (int i = 1; i <= 12; i++) {
            int angle = i * 30;
            int labelX = centerX + (int)((radius - 1) * sin(angle * 3.14159 / 180));
            int labelY = centerY - (int)((radius - 1) * cos(angle * 3.14159 / 180));
            putString(labelX, labelY, to_string(i), LIGHT_GREEN, BLACK);
        }

        for (int i = 1; i <= radius; i++) {
            int x = centerX + (int)(hourX - centerX) * i / radius;
            int y = centerY + (int)(hourY - centerY) * i / radius;
            if (x >= 0 && x < CONSOLE_WIDTH && y >= 0 && y < CONSOLE_HEIGHT) {
                putChar(x, y, '#', LIGHT_RED, BLACK);
            }
        }

        for (int i = 1; i <= radius; i++) {
            int x = centerX + (int)(minuteX - centerX) * i / radius;
            int y = centerY + (int)(minuteY - centerY) * i / radius;
            if (x >= 0 && x < CONSOLE_WIDTH && y >= 0 && y < CONSOLE_HEIGHT) {
                putChar(x, y, '#', LIGHT_GREEN, BLACK);
            }
        }

        if (blinkState) {
            putChar(secondX, secondY, '*', YELLOW, BLACK);
        }
    }
    else {
        putString(20, 10, "Digital Time:", YELLOW, BLACK);
        putString(20, 12, timeStr, LIGHT_CYAN, BLACK);

        char dateStr[30];
        strftime(dateStr, sizeof(dateStr), "%A, %d %B %Y", ltm);
        putString(20, 14, dateStr, LIGHT_GREEN, BLACK);
    }

    drawBox(4, 18, CONSOLE_WIDTH - 44, 8, GREEN, "Clock Features", true);

    putString(6, 20, "Alarm: ", YELLOW, BLACK);
    if (clockData.alarmSet) {
        string alarmStr = "SET for " + to_string(clockData.alarmHour) + ":" +
            (clockData.alarmMinute < 10 ? "0" : "") + to_string(clockData.alarmMinute);
        putString(6 + 7, 20, alarmStr, LIGHT_RED, BLACK);
    }
    else {
        putString(6 + 7, 20, "NOT SET", LIGHT_GRAY, BLACK);
    }

    putString(6, 22, "Timer: ", YELLOW, BLACK);
    if (clockData.timerRunning) {
        time_t elapsed = time(0) - clockData.timerStart;
        int remaining = clockData.timerSeconds - (int)elapsed;
        if (remaining < 0) remaining = 0;
        int minutes = remaining / 60;
        int seconds = remaining % 60;
        string timerStr = to_string(minutes) + ":" + (seconds < 10 ? "0" : "") + to_string(seconds);
        putString(6 + 7, 22, timerStr, LIGHT_CYAN, BLACK);
    }
    else {
        putString(6 + 7, 22, "STOPPED", LIGHT_GRAY, BLACK);
    }

    drawBox(4, 27, CONSOLE_WIDTH - 44, 6, MAGENTA, "Clock Controls", true);
    putString(6, 33, "Ctrl+A: Toggle Analog/Digital", LIGHT_GREEN, BLACK);
    putString(40, 33, "Ctrl+S: Set Alarm", LIGHT_GREEN, BLACK);
    putString(70, 33, "ESC: Back", YELLOW, BLACK);

    putString(4, 35, "Keys: ", YELLOW, BLACK);
    putString(4 + 6, 35, "Ctrl+A=Analog/Digital  Ctrl+S=Set Alarm  Ctrl+T=Timer  Ctrl+R=Reset  ESC=Back", LIGHT_GRAY, BLACK);
}

bool handleClockMode() {
    clearScreen();
    updateScreen();
    needRedraw = true;
    initializeClock();
    needRedraw = true;

    while (true) {
        getSystemInfo();

        if (clock() - lastBlink > 500) {
            blinkState = !blinkState;
            lastBlink = clock();
        }

        printClockApp();
        updateScreen();

        if (clockData.alarmSet) {
            time_t now = time(0);
            tm* ltm = localtime(&now);
            if (ltm->tm_hour == clockData.alarmHour && ltm->tm_min == clockData.alarmMinute && ltm->tm_sec == 0) {
                playSuccessSound();
                printStatusBar("ALARM! ALARM! ALARM!", LIGHT_RED);
                Sleep(3000);
                clockData.alarmSet = false;
                needRedraw = true;
            }
        }

        if (clockData.timerRunning) {
            time_t elapsed = time(0) - clockData.timerStart;
            if (elapsed >= clockData.timerSeconds) {
                clockData.timerRunning = false;
                playSuccessSound();
                printStatusBar("TIMER COMPLETE!", LIGHT_GREEN);
                Sleep(3000);
                needRedraw = true;
            }
        }

        if (_kbhit()) {
            int key = _getch();
            if ((key == 1) && (GetKeyState(VK_CONTROL) & 0x8000)) {
                playSelectSound();
                clockData.showAnalog = !clockData.showAnalog;
                needRedraw = true;
            }
            else if ((key == 19) && (GetKeyState(VK_CONTROL) & 0x8000)) {
                playSelectSound();
                markLineDirty(34);
                putString(6, 34, "Enter alarm time (HH MM): ", LIGHT_GREEN, BLACK);
                updateScreen();

                string input;
                char ch;
                while ((ch = _getch()) != 13) {
                    if (ch == 8 && !input.empty()) {
                        input.pop_back();
                    }
                    else if (isdigit(ch) || ch == ' ') {
                        input += ch;
                    }
                }
                stringstream ss(input);
                int hour, minute;
                if (ss >> hour >> minute) {
                    if (hour >= 0 && hour < 24 && minute >= 0 && minute < 60) {
                        clockData.alarmSet = true;
                        clockData.alarmHour = hour;
                        clockData.alarmMinute = minute;
                        printStatusBar("Alarm set for " + to_string(hour) + ":" +
                            (minute < 10 ? "0" : "") + to_string(minute), LIGHT_GREEN);
                        needRedraw = true;
                    }
                    else {
                        printStatusBar("Invalid time format", LIGHT_RED);
                    }
                }
                markLineDirty(34);
            }
            else if ((key == 20) && (GetKeyState(VK_CONTROL) & 0x8000)) {
                playSelectSound();
                if (clockData.timerRunning) {
                    clockData.timerRunning = false;
                    printStatusBar("Timer stopped", LIGHT_RED);
                }
                else {
                    clockData.timerRunning = true;
                    clockData.timerStart = time(0);
                    printStatusBar("Timer started", LIGHT_GREEN);
                }
                needRedraw = true;
            }
            else if (key == 27) {
                playCloseSound();
                currentScreen = DESKTOP;
                return true;
            }
            else if ((key == 8) && (GetKeyState(VK_CONTROL) & 0x8000)) {
                playSelectSound();
                currentScreen = HELP;
                return true;
            }
            else if ((key == 16) && (GetKeyState(VK_CONTROL) & 0x8000)) {
                playSelectSound();
                systemMonitorVisible = !systemMonitorVisible;
                needRedraw = true;
            }
        }
        Sleep(100);
    }
    return false;
}

void printCPlusCompiler() {
    static string lastCppSourceCode = "";
    static bool lastCompileSuccess = false;
    static string lastCompileOutput = "";
    static int lastCompileErrors = -1;
    static int lastCompileWarnings = -1;

    bool sourceChanged = (lastCppSourceCode != cppSourceCode);
    bool compileChanged = (lastCompileSuccess != lastCompile.success ||
        lastCompileOutput != lastCompile.output ||
        lastCompileErrors != lastCompile.errors ||
        lastCompileWarnings != lastCompile.warnings);

    if (!needRedraw && !sourceChanged && !compileChanged) {
        return;
    }

    lastCppSourceCode = cppSourceCode;
    lastCompileSuccess = lastCompile.success;
    lastCompileOutput = lastCompile.output;
    lastCompileErrors = lastCompile.errors;
    lastCompileWarnings = lastCompile.warnings;

    drawBox(2, 4, CONSOLE_WIDTH - 40, 30, LIGHT_YELLOW, "C++ Compiler", true);

    putString(4, 6, "C++ Code Editor:", YELLOW, BLACK);
    drawBox(4, 7, CONSOLE_WIDTH - 44, 15, BLACK);

    vector<string> lines;
    stringstream ss(cppSourceCode);
    string line;
    while (getline(ss, line)) {
        lines.push_back(line);
    }

    int visibleLines = min(12, (int)lines.size());
    for (int i = 0; i < visibleLines; i++) {
        string lineNum = to_string(i + 1);
        if (lineNum.length() < 3) lineNum = string(3 - lineNum.length(), ' ') + lineNum;
        putString(6, 9 + i, lineNum + " | ", LIGHT_GRAY, BLACK);

        if (i < (int)lines.size()) {
            if (lines[i].find("#include") != string::npos) {
                putString(11, 9 + i, lines[i], LIGHT_CYAN, BLACK);
            }
            else if (lines[i].find("int main") != string::npos || lines[i].find("cout") != string::npos) {
                putString(11, 9 + i, lines[i], LIGHT_GREEN, BLACK);
            }
            else {
                putString(11, 9 + i, lines[i], LIGHT_GRAY, BLACK);
            }
        }
    }

    drawBox(4, 23, CONSOLE_WIDTH - 44, 7, GREEN, "Compiler Output", true);

    putString(6, 25, "Compile Result: ", YELLOW, BLACK);
    if (lastCompile.success) {
        putString(6 + 16, 25, "SUCCESS", LIGHT_GREEN, BLACK);
    }
    else {
        putString(6 + 16, 25, "FAILED", LIGHT_RED, BLACK);
    }

    putString(6, 26, "Errors: ", YELLOW, BLACK);
    putString(6 + 8, 26, to_string(lastCompile.errors), LIGHT_GRAY, BLACK);

    putString(30, 26, "Warnings: ", YELLOW, BLACK);
    putString(30 + 10, 26, to_string(lastCompile.warnings), LIGHT_GRAY, BLACK);

    putString(60, 26, "Time: ", YELLOW, BLACK);
    putString(60 + 6, 26, to_string(lastCompile.time).substr(0, 4) + "s", LIGHT_GRAY, BLACK);

    if (!lastCompile.output.empty()) {
        string shortOutput = lastCompile.output;
        if (shortOutput.length() > 100) {
            shortOutput = shortOutput.substr(0, 97) + "...";
        }
        putString(6, 27, shortOutput, LIGHT_GRAY, BLACK);
    }

    drawBox(4, 31, CONSOLE_WIDTH - 44, 4, CYAN, "Compiler Commands", true);
    putString(6, 32, "Ctrl+C: Compile Code", LIGHT_GREEN, BLACK);
    putString(30, 32, "Ctrl+R: Run Program", LIGHT_GREEN, BLACK);
    putString(60, 32, "Ctrl+S: Save", LIGHT_CYAN, BLACK);
    putString(85, 32, "ESC: Back", YELLOW, BLACK);

    putString(4, 35, "Keys: ", YELLOW, BLACK);
    putString(4 + 6, 35, "CTRL+C=Compile  CTRL+R=Run  CTRL+S=Save  CTRL+L=Load  ESC=Back", LIGHT_GRAY, BLACK);
}

bool handleCPlusCompilerMode() {
    clearScreen();
    updateScreen();
    needRedraw = true;
    initializeCompiler();
    needRedraw = true;

    while (true) {
        getSystemInfo();

        if (clock() - lastBlink > 500) {
            blinkState = !blinkState;
            lastBlink = clock();
        }

        printCPlusCompiler();
        updateScreen();

        if (_kbhit()) {
            int key = _getch();
            if ((key == 3) && (GetKeyState(VK_CONTROL) & 0x8000)) {
                playCompileSound();
                printStatusBar("Compiling code...", LIGHT_CYAN);
                lastCompile = compileCppCode(cppSourceCode);
                compileHistory.push_back(string("Compilation: ") + (lastCompile.success ? "Success" : "Failed"));
                if (lastCompile.success) {
                    playSuccessSound();
                    printStatusBar("Compilation successful!", LIGHT_GREEN);
                }
                else {
                    playErrorSound();
                    printStatusBar("Compilation failed! Check output.", LIGHT_RED);
                }
                needRedraw = true;
                Sleep(1000);
            }
            else if ((key == 18) && (GetKeyState(VK_CONTROL) & 0x8000)) {
                if (lastCompile.success) {
                    playSelectSound();
                    runCompiledProgram();
                }
                else {
                    playErrorSound();
                    printStatusBar("No successful compilation to run", LIGHT_RED);
                    Sleep(1000);
                }
            }
            else if ((key == 19) && (GetKeyState(VK_CONTROL) & 0x8000)) {
                playSelectSound();
                markLineDirty(34);
                putString(6, 34, "Enter filename to save (without .cpp): ", LIGHT_GREEN, BLACK);
                updateScreen();

                string filename;
                char ch;
                while ((ch = _getch()) != 13) {
                    if (ch == 8 && !filename.empty()) {
                        filename.pop_back();
                    }
                    else if (isprint(ch)) {
                        filename += ch;
                    }
                }
                if (!filename.empty()) {
                    saveCppCode(filename);
                }
                markLineDirty(34);
                Sleep(1000);
            }
            else if ((key == 12) && (GetKeyState(VK_CONTROL) & 0x8000)) {
                playSelectSound();
                markLineDirty(34);
                putString(6, 34, "Enter filename to load (without .cpp): ", LIGHT_GREEN, BLACK);
                updateScreen();

                string filename;
                char ch;
                while ((ch = _getch()) != 13) {
                    if (ch == 8 && !filename.empty()) {
                        filename.pop_back();
                    }
                    else if (isprint(ch)) {
                        filename += ch;
                    }
                }
                if (!filename.empty()) {
                    loadCppCode(filename);
                    needRedraw = true;
                }
                markLineDirty(34);
                Sleep(1000);
            }
            else if (key == 27) {
                playCloseSound();
                currentScreen = DESKTOP;
                return true;
            }
            else if ((key == 8) && (GetKeyState(VK_CONTROL) & 0x8000)) {
                playSelectSound();
                currentScreen = HELP;
                return true;
            }
            else if ((key == 16) && (GetKeyState(VK_CONTROL) & 0x8000)) {
                playSelectSound();
                systemMonitorVisible = !systemMonitorVisible;
                needRedraw = true;
            }
            else if (key == 13) {
                playSelectSound();
                cppSourceCode += "\n";
                printStatusBar("New line added", LIGHT_GREEN);
                needRedraw = true;
                Sleep(500);
            }
            else if (isprint(key) || key == 8) {
                playMoveSound();
                if (key == 8 && !cppSourceCode.empty()) {
                    cppSourceCode.pop_back();
                }
                else if (isprint(key)) {
                    cppSourceCode += char(key);
                }
                needRedraw = true;
            }
        }
        Sleep(50);
    }
    return false;
}

void printNewYearQuest() {
    static int lastCurrentStep = -1;
    static int lastQuestSelectedOption = -1;
    static bool lastBlinkState = false;

    bool stepChanged = (lastCurrentStep != questData.currentStep);
    bool optionChanged = (lastQuestSelectedOption != questSelectedOption);
    bool blinkChanged = (lastBlinkState != blinkState);

    if (!needRedraw && !stepChanged && !optionChanged && !blinkChanged) {
        return;
    }

    lastCurrentStep = questData.currentStep;
    lastQuestSelectedOption = questSelectedOption;
    lastBlinkState = blinkState;

    drawBox(2, 4, CONSOLE_WIDTH - 40, 30, LIGHT_YELLOW, "New Year Quest", true);

    putString(4, 6, "Holiday Adventure Game", YELLOW, BLACK);
    drawBox(4, 7, CONSOLE_WIDTH - 44, 15, GREEN);

    if (questData.currentStep < (int)questSteps.size()) {
        putString(6, 8, questSteps[questData.currentStep], LIGHT_CYAN, BLACK);
    }

    if (questData.currentStep == 2) {
        putString(6, 12, "Choose door (1-3):", YELLOW, BLACK);

        if (questSelectedOption == 0) {
            putString(6, 14, "1. Living Room", LIGHT_GREEN, BLACK);
        }
        else {
            putString(6, 14, "1. Living Room", LIGHT_GRAY, BLACK);
        }

        if (questSelectedOption == 1) {
            putString(6, 15, "2. Kitchen", LIGHT_GREEN, BLACK);
        }
        else {
            putString(6, 15, "2. Kitchen", LIGHT_GRAY, BLACK);
        }

        if (questSelectedOption == 2) {
            putString(6, 16, "3. Attic", LIGHT_GREEN, BLACK);
        }
        else {
            putString(6, 16, "3. Attic", LIGHT_GRAY, BLACK);
        }
    }
    else if (questData.currentStep == 4) {
        putString(6, 12, "Enter answer: ", YELLOW, BLACK);
    }

    drawBox(4, 23, CONSOLE_WIDTH - 44, 8, MAGENTA, "Game Progress", true);

    putString(6, 25, "Progress: ", YELLOW, BLACK);
    int progress = (questData.currentStep * 100) / (int)questSteps.size();
    putString(6 + 10, 25, to_string(progress) + "%", LIGHT_GRAY, BLACK);

    putString(6, 26, "Inventory: ", YELLOW, BLACK);
    int invX = 6 + 11;

    if (questData.hasKey) {
        putString(invX, 26, "[Key] ", LIGHT_YELLOW, BLACK);
        invX += 6;
    }
    if (questData.hasGift) {
        putString(invX, 26, "[Gift] ", LIGHT_MAGENTA, BLACK);
        invX += 7;
    }
    if (questData.metSanta) {
        putString(invX, 26, "[Met Santa] ", LIGHT_CYAN, BLACK);
        invX += 12;
    }
    if (questData.puzzleSolved) {
        putString(invX, 26, "[Puzzle Solved] ", LIGHT_GREEN, BLACK);
    }

    putString(6, 27, "Step: ", YELLOW, BLACK);
    putString(6 + 6, 27, to_string(questData.currentStep + 1) + "/" + to_string(questSteps.size()), LIGHT_GRAY, BLACK);

    drawBox(4, 32, CONSOLE_WIDTH - 44, 3, CYAN, "Controls", true);
    putString(6, 33, "ENTER: Continue", LIGHT_GREEN, BLACK);
    putString(40, 33, "ESC: Back", YELLOW, BLACK);

    putString(4, 35, "Keys: ", YELLOW, BLACK);
    putString(4 + 6, 35, "1-3=Select option  ENTER=Continue  ESC=Back", LIGHT_GRAY, BLACK);
}

bool handleNewYearQuestMode() {
    clearScreen();
    updateScreen();
    needRedraw = true;
    initializeNewYearQuest();
    needRedraw = true;

    while (true) {
        getSystemInfo();

        if (clock() - lastBlink > 500) {
            blinkState = !blinkState;
            lastBlink = clock();
        }

        printNewYearQuest();
        updateScreen();

        if (_kbhit()) {
            int key = _getch();
            if (key == '1' && questData.currentStep == 2) {
                playSelectSound();
                questSelectedOption = 0;
            }
            else if (key == '2' && questData.currentStep == 2) {
                playSelectSound();
                questSelectedOption = 1;
            }
            else if (key == '3' && questData.currentStep == 2) {
                playSelectSound();
                questSelectedOption = 2;
            }
            else if (key == 13) {
                playSelectSound();
                if (questData.currentStep == 2) {
                    if (questSelectedOption == 0) {
                        questData.currentStep = 3;
                    }
                    else if (questSelectedOption == 1) {
                        questData.currentStep = 6;
                    }
                    else if (questSelectedOption == 2) {
                        if (questData.hasKey) {
                            questData.currentStep = 8;
                        }
                        else {
                            printStatusBar("The attic is locked! You need a key.", LIGHT_RED);
                            _getch();
                        }
                    }
                }
                else if (questData.currentStep == 4) {
                    questData.puzzleSolved = true;
                    questData.hasKey = true;
                    questData.currentStep = 5;
                    playSuccessSound();
                    printStatusBar("Correct! You found a key under the tree!", LIGHT_GREEN);
                    _getch();
                }
                else if (questData.currentStep < (int)questSteps.size() - 1) {
                    questData.currentStep++;
                }
                else {
                    questData.currentStep = 0;
                    initializeNewYearQuest();
                    playNewYearSong();
                    printStatusBar("Happy New Year! Quest completed!", LIGHT_GREEN);
                    _getch();
                }
                needRedraw = true;
            }
            else if (key == 27) {
                playCloseSound();
                currentScreen = DESKTOP;
                return true;
            }
            else if ((key == 8) && (GetKeyState(VK_CONTROL) & 0x8000)) {
                playSelectSound();
                currentScreen = HELP;
                return true;
            }
            else if ((key == 16) && (GetKeyState(VK_CONTROL) & 0x8000)) {
                playSelectSound();
                systemMonitorVisible = !systemMonitorVisible;
                needRedraw = true;
            }
            else if (isprint(key) && questData.currentStep == 4) {
                playMoveSound();
            }
        }
        Sleep(50);
    }
    return false;
}

int main() {
    setConsoleEncoding();
    setConsoleSize();
    systemSettings.fontName = "Terminal";
    systemSettings.fontSizeX = 8;
    systemSettings.fontSizeY = 16;
    setFont(systemSettings.fontName, systemSettings.fontSizeX, systemSettings.fontSizeY);

    initCPUMonitor();

    showBootScreen();
    initializeApplications();
    initializeTasks();

    bool running = true;
    while (running) {
        // Очищаем экран при каждом переходе между приложениями
        clearScreen();
        updateScreen(); // Принудительно обновляем экран после очистки

        // Сбрасываем флаги перерисовки
        needRedraw = true;

        switch (currentScreen) {
        case DESKTOP:
            running = handleDesktopMode();
            break;
        case CALCULATOR:
            running = handleCalculatorMode();
            break;
        case FILE_MANAGER:
            running = handleFileManagerMode();
            break;
        case TEXT_EDITOR:
            running = handleTextEditorMode();
            break;
        case HELP:
            running = handleHelpMode();
            break;
        case SETTINGS:
            running = handleSettingsMode();
            break;
        case TASK_MANAGER:
            running = handleTaskManagerMode();
            break;
        case MEMORY_GAME:
            running = handleMemoryGameMode();
            break;
        case PROCESS_MONITOR:
            running = handleProcessMonitorMode();
            break;
        case PAINT:
            running = handlePaintMode();
            break;
        case MUSIC_PLAYER:
            running = handleMusicPlayerMode();
            break;
        case WEATHER:
            running = handleWeatherMode();
            break;
        case ARCADE_GAME:
            running = handleArcadeGameMode();
            break;
        case CLOCK:
            running = handleClockMode();
            break;
        case NEW_YEAR_QUEST:
            running = handleNewYearQuestMode();
            break;
        case MUSIC_EDITOR:
            running = handleMusicEditorMode();
            break;
        case RACING_GAME:
            running = handleRacingGameMode();
            break;
        case CPLUS_COMPILER:
            running = handleCPlusCompilerMode();
            break;
        }
    }

    cleanupCPUMonitor();

    clearScreen();
    putString(40, 15, "SOSIOC shutdown complete.", LIGHT_GREEN, BLACK);
    putString(40, 17, "Press any key to exit...", YELLOW, BLACK);
    updateScreen();

    _getch();
    return 0;
}