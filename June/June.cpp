#include <iostream>
#include <windows.h>
#include <conio.h>
#include <cstdlib>
#include <ctime>

using namespace std;

const int TICK = 60;
int tickCounter = 0;

const int viewWidth = 20;
const int viewHeight = 10;

const int start_map_size = 1;

const int EXPAND_SIZE_X = viewWidth / 2;
const int EXPAND_SIZE_Y = viewHeight * 2;

const string TREE = "\033[48;5;22m^\033[0m";
const string COPPER = "\033[48;5;172m#\033[0m";
const string ROCK = "\033[38;5;7m\033[48;5;240m#\033[0m";
const string FLOOR = "\033[48;5;28m \033[0m";
const string IRON = "\033[48;5;240m\033[38;5;255m*\033[0m";
const string GOLD = "\033[48;5;226m\033[38;5;220m*\033[0m";

const string ZOMBIE = "\033[38;5;22m\033[48;5;64m@\033[0m";

int mapWidth = start_map_size;
int mapHeight = start_map_size;

string** map;

struct Item {
    string name;
    int count;
};

struct Player {
    string player_char = "LT Team";
    int health = 90;
    int local_x = 0;
    int local_y = 0;
    int x = start_map_size;
    int y = start_map_size;
    Item backpack[10];
    int backpackSize = 0;
    int levelPoints = 0;
    int damage = 20;
    int backpackCapacity = 10;
};

struct Enemy {
    string enemy_char;
    int health;
    int x;
    int y;
    int damage;
    int detectRadius;
};

const int MOB_CAP = 70;
int mobCount = 0;

Enemy mob_list[MOB_CAP];



enum class direction { UP = '^', DOWN = 'v', LEFT = '<', RIGHT = '>' };

const int SIMULATION_MAP_X = 25;
const int SIMULATION_MAP_Y = 15;

struct CraftItem {
    string icon;
    string name;
    int requiredCount;
    Item requiredItems[10];
    int requiredItemCount;
};

CraftItem craftableItems[10];
int craftableItemCount = 0;

void generateMap(int width, int height) {
    map = new string * [height];
    for (int y = 0; y < height; ++y) {
        map[y] = new string[width];
    }
}

void deleteMap(int width, int height) {
    for (int y = 0; y < height; ++y) {
        delete[] map[y];
    }
    delete[] map;
}

void expandMap(Player& player) {
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
                if (randNum < 1) {
                    if (mobCount < MOB_CAP) {
                        newMap[y][x] = ZOMBIE;
                        mob_list[mobCount] = { ZOMBIE, 100, x, y, 25, 5 };
                        mobCount++;
                    }
                    else {
                        newMap[y][x] = FLOOR;
                    }
                }
                else if (randNum < 6) {
                    newMap[y][x] = ROCK;
                }
                else if (randNum < 10) {
                    newMap[y][x] = TREE;
                }
                else if (randNum < 13) {
                    newMap[y][x] = COPPER;
                }
                else if (randNum < 15) {
                    newMap[y][x] = IRON;
                }
                else if (randNum < 16) {
                    newMap[y][x] = GOLD;
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

bool inRadius(Enemy& mob, Player& player) {
    return abs(mob.x - player.x) <= mob.detectRadius && abs(mob.y - player.y) <= mob.detectRadius;
}

void moveEnemy(Player& player) {
    if (tickCounter % 3 == 0) {
        for (int i = 0; i < mobCount; i++) {
            Enemy& mob = mob_list[i];

            // Уберите зомби с текущей позиции на карте
            map[mob.y][mob.x] = FLOOR;

            if (inRadius(mob, player)) {
                int new_x = mob.x;
                int new_y = mob.y;

                if (mob.x < player.x && mob.x < mapWidth - 1) new_x++;
                else if (mob.x > player.x && mob.x > 0) new_x--;

                if (mob.y < player.y && mob.y < mapHeight - 1) new_y++;
                else if (mob.y > player.y && mob.y > 0) new_y--;

                if (!(new_x == player.x && new_y == player.y)) {
                    mob.x = new_x;
                    mob.y = new_y;
                }
            }
            else {
                int direction = rand() % 4;
                int new_x = mob.x;
                int new_y = mob.y;

                switch (direction) {
                case 0: if (mob.x > 0) new_x--; break;
                case 1: if (mob.x < mapWidth - 1) new_x++; break;
                case 2: if (mob.y > 0) new_y--; break;
                case 3: if (mob.y < mapHeight - 1) new_y++; break;
                }

                if (!(new_x == player.x && new_y == player.y)) {
                    mob.x = new_x;
                    mob.y = new_y;
                }
            }

            // Поместите зомби на новую позицию на карте
            map[mob.y][mob.x] = ZOMBIE;
        }
    }
}

void backpackMenu(Player& player) {
    while (true) {
        system("cls");
        cout << "\033[38;2;158;100;0m   __________                  __    __________                  __    \n\\______   \\_____     ____  |  | __\\______   \\_____     ____  |  | __\n |    |  _/\\__  \\  _/ ___\\ |  |/ / |     ___/\\__  \\  _/ ___\\ |  |/ /\n |    |   \\ / __ \\_\\  \\___ |    <  |    |     / __ \\_\\  \\___ |    < \n |______  /(____  / \\___  >|__|_ \\ |____|    (____  / \\___  >|__|_ \\\n        \\/      \\/      \\/      \\/                \\/      \\/      \\/\033[0m";
        for (int i = 0; i < player.backpackSize; ++i) {
            cout << player.backpack[i].name << " x" << player.backpack[i].count << "\n";
        }

        switch (_getch()) {
        case 'w':
            break;
        default:
            break;
        }
        break;
    }
}

void addItem(Player& player, string item_name) {
    for (int i = 0; i < player.backpackSize; ++i) {
        if (player.backpack[i].name == item_name) {
            player.backpack[i].count++;
            return;
        }
    }

    if (player.backpackSize < player.backpackCapacity) {
        player.backpack[player.backpackSize] = { item_name, 1 };
        player.backpackSize++;
    }
    else {
        cout << "Your backpack is full!\n";
    }
}




void craftItem(Player& player, CraftItem& craftItem) {
    bool canCraft = true;

    for (int i = 0; i < craftItem.requiredItemCount; ++i) {
        bool found = false;
        for (int j = 0; j < player.backpackSize; ++j) {
            if (player.backpack[j].name == craftItem.requiredItems[i].name &&
                player.backpack[j].count >= craftItem.requiredItems[i].count) {
                found = true;
                break;
            }
        }
        if (!found) {
            canCraft = false;
            break;
        }
    }

    if (canCraft) {
        for (int i = 0; i < craftItem.requiredItemCount; ++i) {
            for (int j = 0; j < player.backpackSize; ++j) {
                if (player.backpack[j].name == craftItem.requiredItems[i].name) {
                    player.backpack[j].count -= craftItem.requiredItems[i].count;
                    if (player.backpack[j].count == 0) {
                        for (int k = j; k < player.backpackSize - 1; ++k) {
                            player.backpack[k] = player.backpack[k + 1];
                        }
                        player.backpackSize--;
                    }
                    break;
                }
            }
        }
        addItem(player, craftItem.name);
    }
}

void craftingMenu(Player& player) {
    while (true) {
        system("cls");
        cout << "\033[38;2;158;100;0m   ___________                .__  __             \n\\__    ___/___   ____    __|__|/  |_   ____    \n  |    | /  _ \\ /    \\  /  _ \\  \\   __\\_/ __ \\   \n  |    |(  <_> )   |  (  <_> )  ||  |  \\  ___/   \n  |____| \\____/|___|  / \\____/|__||__|   \\___  >  \n                   \\/                       \\/  \033[0m";
        cout << "Select an item to craft:\n";
        for (int i = 0; i < craftableItemCount; ++i) {
            cout << i + 1 << ". " << craftableItems[i].name << "\n";
        }
        cout << craftableItemCount + 1 << ". Exit\n";

        int choice;
        cin >> choice;

        if (choice == craftableItemCount + 1) {
            break;
        }

        if (choice > 0 && choice <= craftableItemCount) {
            CraftItem item = craftableItems[choice - 1];
            bool canCraft = true;

            for (int i = 0; i < item.requiredItemCount; ++i) {
                bool found = false;
                for (int j = 0; j < player.backpackSize; ++j) {
                    if (player.backpack[j].name == item.requiredItems[i].name && player.backpack[j].count >= item.requiredItems[i].count) {
                        found = true;
                        break;
                    }
                }
                if (!found) {
                    canCraft = false;
                    break;
                }
            }

            if (canCraft) {
                for (int i = 0; i < item.requiredItemCount; ++i) {
                    for (int j = 0; j < player.backpackSize; ++j) {
                        if (player.backpack[j].name == item.requiredItems[i].name) {
                            player.backpack[j].count -= item.requiredItems[i].count;
                            if (player.backpack[j].count == 0) {
                                for (int k = j; k < player.backpackSize - 1; ++k) {
                                    player.backpack[k] = player.backpack[k + 1];
                                }
                                player.backpackSize--;
                            }
                            break;
                        }
                    }
                }
                addItem(player, item.name);
                cout << "Crafted " << item.name << "!\n";
            }
            else {
                cout << "Not enough resources to craft " << item.name << "!\n";
            }
        }
    }
}

void attackEnemy(Player& player) {
    int attackRange = 2;
    for (int i = 0; i < mobCount; ++i) {
        Enemy& mob = mob_list[i];
        if (abs(mob.x - player.x) <= attackRange && abs(mob.y - player.y) <= attackRange) {
            mob.health -= 10;
            if (mob.health <= 0) {

                map[mob.y][mob.x] = FLOOR;

                for (int j = i; j < mobCount - 1; ++j) {
                    mob_list[j] = mob_list[j + 1];
                }
                mobCount--;
                player.levelPoints += 1;
            }
            break;
        }
    }
}

void destroy(Player& player) {
    int x = player.x;
    int y = player.y;

    if (player.player_char == "^" && y > 0) y--;
    else if (player.player_char == "v" && y < mapHeight - 1) y++;
    else if (player.player_char == ">" && x < mapWidth - 1) x++;
    else if (player.player_char == "<" && x > 0) x--;

    if (map[y][x] != FLOOR) {
        if (map[y][x] == ZOMBIE) attackEnemy(player);
        else {
            addItem(player, map[y][x]);
            map[y][x] = FLOOR;
        }
    }
}

void levelUpMenu(Player& player) {
    int user = 0, choice = 0;
    while (user != 27) {

        system("cls");
        switch (user) {
        case 27:
            break;
        case 'w':
        case 'W':
        case 72:
            if (0 < choice)choice--;
            break;
        case 's':
        case 'S':
        case 80:
            if (choice < 3)choice++;
            break;
        case 13:
            if (player.levelPoints > 0) {
                switch (choice)
                {
                case 0:
                    player.health += 10;
                    break;
                case 1:
                    player.damage += 5;
                    break;
                case 2:
                    player.backpackCapacity += 3;
                    break;
                default:
                    break;
                }
                player.levelPoints--;
            }
        }
        cout << "You have " << player.levelPoints << " level points.\n";
        for (int i = 0; i < 3; i++) {
            switch (i)
            {
            case 0:
                if (choice == 0) cout << "\033[48;2;100;100;100m";
                cout << "1. Increase health (Current: " << player.health << ")\n";
                cout << "\033[0m";
                break;
            case 1:
                if (choice == 1) cout << "\033[48;2;100;100;100m";
                cout << "2. Increase damage (Current: " << player.damage << ")\n";
                cout << "\033[0m";
                break;
            case 2:
                if (choice == 2) cout << "\033[48;2;100;100;100m";
                cout << "3. Increase backpack capacity (Current: " << player.backpackCapacity << ")\n";
                cout << "\033[0m";
                break;
            default:
                break;
            }
        }
        user = _getch();

    }
}

void movePlayer(Player& player, int move) {
    switch (move) {
    case 'W':
    case 'w':
        if (player.y > 0) player.y--;
        player.player_char = "^";
        player.local_y--;
        break;
    case 'S':
    case 's':
        if (player.y < mapHeight - 1) player.y++;
        player.player_char = "v";
        player.local_y++;
        break;
    case 'A':
    case 'a':
        if (player.x > 0) player.x--;
        player.player_char = "<";
        player.local_x--;
        break;
    case 'D':
    case 'd':
        if (player.x < mapWidth - 1) player.x++;
        player.player_char = ">";
        player.local_x++;
        break;
    case 'B':
    case 'b':
        backpackMenu(player);
        break;
    case 'C':
    case 'c':
        craftingMenu(player);
        break;
    case 'L':
    case 'l':
        levelUpMenu(player);
        break;
    case 72: // Вверх стрелка
        player.player_char = "^";
        break;
    case 80: // Вниз стрелка почему это p(анг)...
        player.player_char = "v";
        break;
    case 75: // Влево стрелка
        player.player_char = "<";
        break;
    case 77: // Вправо стрелка
        player.player_char = ">";
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
    system("cls");
    for (int y = player.y - viewHeight / 2; y < player.y + viewHeight / 2; ++y) {
        for (int x = player.x - viewWidth / 2; x < player.x + viewWidth / 2; ++x) {
            if (x == player.x && y == player.y) {
                cout << player.player_char;
            }
            else if (x >= 0 && x < mapWidth && y >= 0 && y < mapHeight) {
                cout << map[y][x];
            }

        }
        cout << endl;
    }
    moveEnemy(player);
}

void gui(Player& player) {
    cout << "\033[48;2;0;255;0m\033[38;2;255;0;0m";
    cout << player.health;
    for (int i = 0; i < player.health; i++) {
        if (i % 5 == 0) {
            cout << " ";
        }
    }
    cout << "\033[0m";
    cout << endl;
}




void initializeCraftableItems() {

    craftableItems[craftableItemCount] = { "\033[38;5;94m&\033[0m", "Crafting Table", 6, {{"Wood", 4}, {"Stick", 2}}, 2 };

    craftableItems[craftableItemCount] = { "\033[38;5;240m#\033[0m", "Furnace", 8, {{"Cobblestone", 8}}, 1 };

    craftableItems[craftableItemCount] = { "\033[38;5;220m!\033[0m", "Sword", 2, {{"Iron", 2}}, 1 };
    craftableItems[craftableItemCount] = { "\033[38;5;94m(\033[0m", "Armor", 4, {{"Iron", 4}}, 1 };
    craftableItemCount = 4;
}


void start() {

    int move = ' ';

    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);

    srand(time(NULL));
    generateMap(start_map_size, start_map_size);
    Player player;
    initializeCraftableItems();


    while (true) {
        if (tickCounter > TICK) tickCounter = 0;
        printMap(player);
        if (_kbhit()) {
            move = _getch();
            movePlayer(player, move);
        }
        gui(player);
        cout << endl << "lX: " << player.local_x << " | lY: " << player.local_y;
        cout << endl << "gX: " << player.x << " | gY: " << player.y;
        cout << endl << "XM: " << mapWidth << " | YM: " << mapHeight;
        cout << endl << "Memory_Map: " << ((mapHeight * mapWidth) / 1024 / 1024);
        cout << endl << "move: " << move << endl;
        tickCounter++;
        Sleep(1000 / TICK);

    }

    deleteMap(mapWidth, mapHeight);
    delete[] player.backpack;
}

int main() {
    start();
    return 0;
}
