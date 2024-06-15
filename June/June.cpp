#include <iostream>
#include <windows.h>
#include <conio.h>
using namespace std;

// Данные карты
const int start_map_size = 1;
const int VIEW_WIDTH = 20;
const int VIEW_HEIGHT = 10;
const int EXPAND_SIZE_X = VIEW_WIDTH / 2;
const int EXPAND_SIZE_Y = VIEW_HEIGHT / 2;

const char TREE = '^';
const char COPPER = '#';
const char FLOOR = ' ';

char** map;
int mapWidth = start_map_size;
int mapHeight = start_map_size;

// Данные игрока
string* backpack;
char player_char = 'P'; // Using a single character for the player
enum class direction { UP = '^', DOWN = 'v', LEFT = '<', RIGHT = '>' };

int local_x;
int local_y;

// Методы Карты
void updateMap(int width, int height) {
    map = new char* [height];
    for (int y = 0; y < height; ++y) {
        map[y] = new char[width];
        for (int x = 0; x < width; ++x) {
            map[y][x] = FLOOR;
        }
    }
}

void deleteMap(int width, int height) {
    for (int y = 0; y < height; ++y) {
        delete[] map[y];
    }
    delete[] map;
}

void expandMap(int player[2]) {
    int newWidth = mapWidth + 2 * EXPAND_SIZE_X;
    int newHeight = mapHeight + 2 * EXPAND_SIZE_Y;

    char** newMap = new char* [newHeight];
    for (int y = 0; y < newHeight; ++y) {
        newMap[y] = new char[newWidth];
    }

    for (int y = 0; y < newHeight; ++y) {
        for (int x = 0; x < newWidth; ++x) {
            if (y >= EXPAND_SIZE_Y && y < mapHeight + EXPAND_SIZE_Y && x >= EXPAND_SIZE_X && x < mapWidth + EXPAND_SIZE_X) {
                newMap[y][x] = map[y - EXPAND_SIZE_Y][x - EXPAND_SIZE_X];
            }
            else {
                int randNum = rand() % 100;
                if (randNum < 10) {
                    newMap[y][x] = TREE;
                }
                else if (randNum < 11) {
                    newMap[y][x] = COPPER;
                }
                else {
                    newMap[y][x] = FLOOR;
                }
            }
        }
    }

    deleteMap(mapWidth, mapHeight);

    map = newMap;
    mapWidth = newWidth;
    mapHeight = newHeight;

    player[0] += EXPAND_SIZE_X;
    player[1] += EXPAND_SIZE_Y;
}

void printMap(int player[2]) {
    int startX = max(0, player[0] - VIEW_WIDTH / 2);
    int startY = max(0, player[1] - VIEW_HEIGHT / 2);
    int endX = min(mapWidth, startX + VIEW_WIDTH);
    int endY = min(mapHeight, startY + VIEW_HEIGHT);

    for (int y = startY; y < endY; ++y) {
        for (int x = startX; x < endX; ++x) {
            if (x == player[0] && y == player[1]) {
                cout << "\033[38;5;20m\033[48;5;1m" << player_char << "\033[0m";
            }
            else {
                switch (map[y][x]) {
                case TREE:
                    cout << "\033[48;5;22m" << TREE << "\033[0m";
                    break;
                case COPPER:
                    cout << "\033[48;5;172m" << COPPER << "\033[0m";
                    break;
                case FLOOR:
                    cout << "\033[48;5;28m \033[0m";
                    break;
                default:
                    cout << "\033[48;5;28m \033[0m";
                    break;
                }
            }
        }
        cout << endl;
    }
}

// Методы Игрока
void backpack_menu() {
    while (_getch() != 27) {
        system("cls");
        cout << _getch() << endl;
        cout << "\033[38;2;158;100;0m __________                  __    __________                  __    \n\\______   \\_____     ____  |  | __\\______   \\_____     ____  |  | __\n |    |  _/\\__  \\  _/ ___\\ |  |/ / |     ___/\\__  \\  _/ ___\\ |  |/ /\n |    |   \\ / __ \\_\\  \\___ |    <  |    |     / __ \\_\\  \\___ |    < \n |______  /(____  / \\___  >|__|_ \\ |____|    (____  / \\___  >|__|_ \\\n        \\/      \\/      \\/      \\/                \\/      \\/      \\/\033[0m";
    }
}

// 75 <- 77 ->  72 ^ 80 |
void movePlayer(int player[2], char move) {
    switch (move) {
    case 'w':
        if (player[1] > 0) player[1]--;
        player_char = '^';
        local_y--;
        break;
    case 's':
        if (player[1] < mapHeight - 1) player[1]++;
        player_char = 'v';
        local_y++;
        break;
    case 'a':
        if (player[0] > 0) player[0]--;
        player_char = '<';
        local_x--;
        break;
    case 'd':
        if (player[0] < mapWidth - 1) player[0]++;
        player_char = '>';
        local_x++;
        break;
    case 'b':
        backpack_menu();
        break;
    default:
        break;
    }

    if (player[0] <= EXPAND_SIZE_X || player[1] <= EXPAND_SIZE_Y || player[0] >= mapWidth - EXPAND_SIZE_X || player[1] >= mapHeight - EXPAND_SIZE_Y) {
        expandMap(player);
    }
}

int main() {
    // Enable UTF-8 in the console
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);

    srand(time(NULL));
    updateMap(start_map_size, start_map_size);
    int player[2] = { start_map_size / 2, start_map_size / 2 };
    local_x = player[0]; local_y = player[1];
    while (true) {
        system("cls");
        printMap(player);
        cout << endl << "lX: " << local_x << " | lY: " << local_y;
        cout << endl << "gX: " << player[0] << " | gY: " << player[1];
        cout << endl << "XM: " << mapWidth << " | YM: " << mapHeight;
        cout << endl << "Memory_Map: " << ((mapHeight * mapWidth) / 1024 / 1024);

        if (_kbhit()) {
            char move = _getch();
            if (move == 0 || move == 224) { // Handle special keys
                move = _getch();
                switch (move) {
                case 72:
                    movePlayer(player, 'w');
                    break;
                case 80:
                    movePlayer(player, 's');
                    break;
                case 75:
                    movePlayer(player, 'a');
                    break;
                case 77:
                    movePlayer(player, 'd');
                    break;
                default:
                    break;
                }
            }
            else {
                movePlayer(player, tolower(move));
            }
        }
    }

    deleteMap(mapWidth, mapHeight);
    return 0;
}
