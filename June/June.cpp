#include <iostream>
#include <windows.h>
#include <ctime>
#include <conio.h>
using namespace std;

const int start_map_size = 40;
const int VIEW_WIDTH = 40;
const int VIEW_HEIGHT = 20;

const char TREE = '^';
const char COPPER = '#';
const char FLOOR = ' ';

char** map;
int mapWidth = start_map_size;
int mapHeight = start_map_size;

void updateMap(int width, int height) {
    map = new char* [height];
    for (int y = 0; y < height; ++y) {
        map[y] = new char[width];
    }
}

void deleteMap(int width, int height) {
    for (int y = 0; y < height; ++y) {
        delete[] map[y];
    }
    delete[] map;
}

void generateMap(int width, int height) {
    srand(time(0));

    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            int randNum = rand() % 100;
            if (randNum < 10) {
                map[y][x] = TREE;
            }
            else if (randNum < 30) {
                map[y][x] = COPPER;
            }
            else {
                map[y][x] = FLOOR;
            }
        }
    }
}

void expandMap(int player[2]) {
    int newWidth = mapWidth * 2;
    int newHeight = mapHeight * 2;

    char** newMap = new char* [newHeight];
    for (int y = 0; y < newHeight; ++y) {
        newMap[y] = new char[newWidth];
    }

    for (int y = 0; y < newHeight; ++y) {
        for (int x = 0; x < newWidth; ++x) {
            if (y < mapHeight && x < mapWidth) {
                newMap[y][x] = map[y][x];
            }
            else {
                int randNum = rand() % 100;
                if (randNum < 10) {
                    newMap[y][x] = TREE;
                }
                else if (randNum < 30) {
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

    player[0] += mapWidth / 4;
    player[1] += mapHeight / 4;
}

void printMap(int player[2]) {
    int startX = max(0, player[0] - VIEW_WIDTH / 2);
    int startY = max(0, player[1] - VIEW_HEIGHT / 2);
    int endX = min(mapWidth, startX + VIEW_WIDTH);
    int endY = min(mapHeight, startY + VIEW_HEIGHT);

    for (int y = startY; y < endY; ++y) {
        for (int x = startX; x < endX; ++x) {
            if (x == player[0] && y == player[1]) {
                cout << "\033[38;5;20m\033[48;5;1mP\033[0m";
            }
            else {
                switch (map[y][x]) {
                case TREE:
                    cout << "\033[48;5;22m \033[0m";
                    break;
                case COPPER:
                    cout << "\033[48;5;172m \033[0m";
                    break;
                case FLOOR:
                    cout << "\033[48;5;28m \033[0m";
                    break;
               
                }
            }
        }
        cout << endl;
    }
}

void movePlayer(int player[2], char move) {
    switch (move) {
    case 'w':
        if (player[1] > 0) player[1]--;
        break;
    case 's':
        if (player[1] < mapHeight - 1) player[1]++;
        break;
    case 'a':
        if (player[0] > 0) player[0]--;
        break;
    case 'd':
        if (player[0] < mapWidth - 1) player[0]++;
        break;
    }

    if (player[0] <= VIEW_WIDTH / 2 || player[1] <= VIEW_HEIGHT / 2 ||
        player[0] >= mapWidth - VIEW_WIDTH / 2 || player[1] >= mapHeight - VIEW_HEIGHT / 2) {
        expandMap(player);
    }
}

int main() {
    updateMap(start_map_size, start_map_size);
    generateMap(start_map_size, start_map_size);
    //Старт Позиция
    int player[2] = { 100,0 };

    while (true) {
        system("cls");
        printMap(player);
        cout << endl << "X: " << (int)player[0] << " | Y: " << (int)player[1];
        char move = _getch();
        movePlayer(player, move);
        
    }

    deleteMap(mapWidth, mapHeight);
    return 0;
}
