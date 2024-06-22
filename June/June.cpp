#include <iostream>
#include <windows.h>
#include <conio.h>

using namespace std;

const int TICK = 60;
int tickCounter = 0;
// Данные карты
int vievWidth = 104;
int viewHeight = 10;

const int start_map_size = 1;

const int EXPAND_SIZE_X = vievWidth / 2;
const int EXPAND_SIZE_Y = viewHeight * 2;

const string TREE = "\033[48;5;22m^\033[0m";
const string COPPER = "\033[48;5;172m#\033[0m";
const string FLOOR = "\033[48;5;28m \033[0m";
const string ZOMBIE = "\033[38;5;22m\033[48;5;64m@\033[0m";

int mapWidth = start_map_size;
int mapHeight = start_map_size;

string** map;


struct Item {
    string name;
    int count;
};

struct Player {
    string player_char = "LT Team";;
    int health = 100;
    int local_x = 0;
    int local_y = 0;
    int x = start_map_size;
    int y = start_map_size;
    Item* backpack = new Item[0];;
    int backpack_size = 0;
};






// Данные игрока
enum class direction { UP = '^', DOWN = 'v', LEFT = '<', RIGHT = '>' };


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

void expandMap(Player &player) {
    int newWidth = mapWidth + 2 * EXPAND_SIZE_X;
    int newHeight = mapHeight + 2 * EXPAND_SIZE_Y;
    //Для своей устоновки координат
    if (newWidth < player.x) {
        newWidth += player.x;
    }
    if (newHeight < player.y) {
        newHeight += player.y;
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

    player.x += EXPAND_SIZE_X;
    player.y += EXPAND_SIZE_Y;

}



// Методы Игрока
void backpackMenu(Player &player) {
    while (true) {
        system("cls");
        cout << "\t\033[38;2;158;100;0m   __________                  __    __________                  __    \n\\______   \\_____     ____  |  | __\\______   \\_____     ____  |  | __\n |    |  _/\\__  \\  _/ ___\\ |  |/ / |     ___/\\__  \\  _/ ___\\ |  |/ /\n |    |   \\ / __ \\_\\  \\___ |    <  |    |     / __ \\_\\  \\___ |    < \n |______  /(____  / \\___  >|__|_ \\ |____|    (____  / \\___  >|__|_ \\\n        \\/      \\/      \\/      \\/                \\/      \\/      \\/\033[0m";
        cout << "\n\nBackpack Contents:\n";
        for (int i = 0; i < player.backpack_size; ++i) {
            
        }
       
        switch (_getch())
        {
        case 'w':
            break;
        default:
            break;
        }
        break;
    }
}

void addItem(Player &player, string item_name) {
    for (int i = 0; i < player.backpack_size; ++i) {
        if (player.backpack[i].name == item_name) {
            player.backpack[i].count++;
            return;
        }
    }
    Item* new_backpack = new Item[player.backpack_size + 1];
    for (int i = 0; i < player.backpack_size; ++i) {
        new_backpack[i] = player.backpack[i];
    }
    new_backpack[player.backpack_size].name = item_name;
    new_backpack[player.backpack_size].count = 1;
    delete[] player.backpack;
    player.backpack = new_backpack;
    player.backpack_size++;
}

void destroy(Player &player) {
    int x = player.x;
    int y = player.y;

    if (player.player_char == "^" && y > 0) y--;
    else if (player.player_char == "v" && y < mapHeight - 1) y++;
    else if (player.player_char == ">" && x < mapWidth - 1) x++;
    else if (player.player_char == "<" && x > 0) x--;

    if (map[y][x] != FLOOR) {
        addItem(player, map[y][x]);
        map[y][x] = FLOOR;
    }
}

void movePlayer(Player &player, int move) {
    switch (move) {
    case 'W':
    case 'w':
        if (player.y > 0) player.y--;
        player.player_char = (char)direction::UP;
        player.local_y--;
        break;
    case 'S':
    case 's':
        if (player.y < mapHeight - 1) player.y++;
        player.player_char = (char)direction::DOWN;
        player.local_y++;
        break;
    case 'A':
    case 'a':
        if (player.x > 0) player.x--;
        player.player_char = (char)direction::LEFT;
        player.local_x--;
        break;
    case 'D':
    case 'd':
        if (player.x < mapWidth - 1) player.x++;
        player.player_char = (char)direction::RIGHT;
        player.local_x++;
        break;
    case 'B':
    case 'b':
        backpackMenu(player);
        break;
    case 72: // Вверх стрелка
        player.player_char = (char)direction::UP;
        break;
    case 80: // Вниз стрелка
        player.player_char = (char)direction::DOWN;
        break;
    case 75: // Влево стрелка
        player.player_char = (char)direction::LEFT;
        break;
    case 77: // Вправо стрелка
        player.player_char = (char)direction::RIGHT;
        break;
    case 32:
        destroy(player);
        break;
    default:
        break;
    }

    if (player.x <= EXPAND_SIZE_X || player.y <= EXPAND_SIZE_Y || player.x >= mapWidth - EXPAND_SIZE_X || player.y >= mapHeight - EXPAND_SIZE_Y) {
        expandMap(player);
    }
}

void printMap(Player& player) {
    if (tickCounter % 2 == 0) {
        system("cls");
        for (int y = player.y - viewHeight; y < player.y + viewHeight; ++y) {
            for (int x = player.x - vievWidth; x < player.x + vievWidth; ++x) {
                if (x == player.x && y == player.y) {
                    cout << player.player_char;
                }
                else if (x >= 0 && x < mapWidth && y >= 0 && y < mapHeight) {
                    cout << map[y][x];
                }
                else {
                    cout << FLOOR;
                }
            }
            cout << endl;
        }
    }
}

void start() {
    cout << "\033[37,5,4m";
    int move = ' ';

    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);

    srand(time(NULL));
    generateMap(start_map_size, start_map_size);
    Player player;
   


    while (true) {
        if (tickCounter == TICK) tickCounter = 0;
        
        printMap(player);
        if (_kbhit()) {
            move = _getch();
            movePlayer(player, move);
        }

        cout << endl << "lX: " << player.local_x << " | lY: " << player.local_y;
        cout << endl << "gX: " << player.x << " | gY: " << player.y;
        cout << endl << "XM: " << mapWidth << " | YM: " << mapHeight;
        cout << endl << "Memory_Map: " << ((mapHeight * mapWidth) / 1024 / 1024);
        cout << endl << "Tick: " << tickCounter << endl;
        Sleep(1000 / TICK);
        tickCounter++;
    }

    deleteMap(mapWidth, mapHeight);
    delete[] player.backpack;
}
int main() {
    start();
    return 0;
}
