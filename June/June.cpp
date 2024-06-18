#include <iostream>
#include <windows.h>
#include <conio.h>

using namespace std;

const int TICK = 30;
int tickCounter = 0;
// Данные карты
int vievWidth = 20;
int viewHeight = 10;

const int start_map_size = 1;

const int EXPAND_SIZE_X = vievWidth / 2;
const int EXPAND_SIZE_Y = viewHeight / 2;

const string TREE = "\033[48;5;22m^\033[0m";
const string COPPER = "\033[48;5;172m#\033[0m";
const string FLOOR = "\033[48;5;28m \033[0m";
const string ZOMBIE = "\033[38;5;22m\033[48;5;64m@\033[0m";

struct Item {
    string name;
    int count;
};

struct Player {
    string player_char;
    int health = 100;
    int local_x;
    int local_y;
    int x;
    int y;

};

Item* backpack;
int backpack_size = 0;

string** map;
int mapWidth = start_map_size;
int mapHeight = start_map_size;

// Данные игрока
string player_char = "LT Team";
int health = 100; //Добавить бар?
enum class direction { UP = '^', DOWN = 'v', LEFT = '<', RIGHT = '>' };

int local_x;
int local_y;

const int SIMULATION_MAP_X = 25;
const int SIMULATION_MAP_y = 15;

// Методы Карты
void generateMap(int width, int height) {
    map = new string * [height];
    for (int y = 0; y < height; ++y) {
        map[y] = new string[width];
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
    //Для своей устоновки координат
    if (newWidth < player[0]) {
        newWidth += player[0];
    }
    if (newHeight < player[1]) {
        newHeight += player[1];
    }
    string** newMap = new string * [newHeight];
    for (int y = 0; y < newHeight; ++y) {
        newMap[y] = new string[newWidth];
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
    int startX = max(0, player[0] - vievWidth / 2);
    int startY = max(0, player[1] - viewHeight / 2);
    int endX = min(mapWidth, startX + vievWidth);
    int endY = min(mapHeight, startY + viewHeight);

    for (int y = startY; y < endY; ++y) {
        for (int x = startX; x < endX; ++x) {
            if (x == player[0] && y == player[1]) {
                cout << "\033[38;5;20m\033[48;5;1m\033[1m" << player_char << "\033[0m";
            }
            else {
                cout << map[y][x];
            }
        }
        cout << endl;
    }
}

// Методы Игрока
void backpackMenu() {
    while (true) {
        system("cls");
        cout << "\033[38;2;158;100;0m   __________                  __    __________                  __    \n\\______   \\_____     ____  |  | __\\______   \\_____     ____  |  | __\n |    |  _/\\__  \\  _/ ___\\ |  |/ / |     ___/\\__  \\  _/ ___\\ |  |/ /\n |    |   \\ / __ \\_\\  \\___ |    <  |    |     / __ \\_\\  \\___ |    < \n |______  /(____  / \\___  >|__|_ \\ |____|    (____  / \\___  >|__|_ \\\n        \\/      \\/      \\/      \\/                \\/      \\/      \\/\033[0m";
        cout << "\n\nBackpack Contents:\n";
        for (int i = 0; i < backpack_size; ++i) {
            
        }
       
        switch (_getch())
        {
        default:
            break;
        }
        break;
    }
}

void addItem(string item_name) {
    for (int i = 0; i < backpack_size; ++i) {
        if (backpack[i].name == item_name) {
            backpack[i].count++;
            return;
        }
    }
    Item* new_backpack = new Item[backpack_size + 1];
    for (int i = 0; i < backpack_size; ++i) {
        new_backpack[i] = backpack[i];
    }
    new_backpack[backpack_size].name = item_name;
    new_backpack[backpack_size].count = 1;
    delete[] backpack;
    backpack = new_backpack;
    backpack_size++;
}

void destroy(int player[2]) {
    int x = player[0];
    int y = player[1];

    if (player_char == "^" && y > 0) y--;
    else if (player_char == "v" && y < mapHeight - 1) y++;
    else if (player_char == ">" && x < mapWidth - 1) x++;
    else if (player_char == "<" && x > 0) x--;

    if (map[y][x] != FLOOR) {
        addItem(map[y][x]);
        map[y][x] = FLOOR;
    }
}

void movePlayer(int player[2], int move) {
    switch (move) {
    case 'W':
    case 'w':
        if (player[1] > 0) player[1]--;
        player_char = (char)direction::UP;
        local_y--;
        break;
    case 'S':
    case 's':
        if (player[1] < mapHeight - 1) player[1]++;
        player_char = (char)direction::DOWN;
        local_y++;
        break;
    case 'A':
    case 'a':
        if (player[0] > 0) player[0]--;
        player_char = (char)direction::LEFT;
        local_x--;
        break;
    case 'D':
    case 'd':
        if (player[0] < mapWidth - 1) player[0]++;
        player_char = (char)direction::RIGHT;
        local_x++;
        break;
    case 'B':
    case 'b':
        backpackMenu();
        break;
    case 72: // Вверх стрелка
        player_char = (char)direction::UP;
        break;
    case 80: // Вниз стрелка
        player_char = (char)direction::DOWN;
        break;
    case 75: // Влево стрелка
        player_char = (char)direction::LEFT;
        break;
    case 77: // Вправо стрелка
        player_char = (char)direction::RIGHT;
        break;
    case 32:
        destroy(player);
        break;
    default:
        break;
    }

    if (player[0] <= EXPAND_SIZE_X || player[1] <= EXPAND_SIZE_Y || player[0] >= mapWidth - EXPAND_SIZE_X || player[1] >= mapHeight - EXPAND_SIZE_Y) {
        expandMap(player);
    }
}

void start() {
    cout << "\033[37,5,4m";
    int move = ' ';

    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);

    srand(time(NULL));
    generateMap(start_map_size, start_map_size);
    int player[2] = { start_map_size / 2, start_map_size / 2 };
    local_x = player[0]; local_y = player[1];

    backpack = new Item[0];

    while (true) {
        if (tickCounter == TICK) tickCounter = 0;
        system("cls");
        printMap(player);
        if (_kbhit()) {
            move = _getch();
            movePlayer(player, move);
        }

        cout << endl << "lX: " << local_x << " | lY: " << local_y;
        cout << endl << "gX: " << player[0] << " | gY: " << player[1];
        cout << endl << "XM: " << mapWidth << " | YM: " << mapHeight;
        cout << endl << "Memory_Map: " << ((mapHeight * mapWidth) / 1024 / 1024);
        cout << endl << "Tick: " << tickCounter;
        Sleep(1000 / TICK);
        tickCounter++;
    }

    deleteMap(mapWidth, mapHeight);
    delete[] backpack;
}
int main() {
    start();
    return 0;
}
