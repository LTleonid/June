#include <iostream>
#include <conio.h>
#include <windows.h> // For system("CLS")

using namespace std;

#define screen_x 120
#define screen_y 29
#define map_x 15
#define map_y 15

void map_render(int* player_pos, int map_size_x, int map_size_y, char map[map_x][map_y], char player_char) {
    int distance = 0;
    for (int i = 0; i < map_size_x; i++) {
        for (int j = 0; j < map_size_y; j++) {
            map[i][j] = '.';
        }
    }
    map[player_pos[0]][player_pos[1]] = player_char;
    map[2][2] = '#';

    for (int i = 1; i <= player_pos[0]; i++) {
        if (map[player_pos[0] - i][player_pos[1]] != '#') {
            if (player_pos[0] - i >= 0) {
                map[player_pos[0] - i][player_pos[1]] = '@';
            }
        }
        else {
            distance = i;
            break;
        }
    }
    
    
    
    if (distance) {
    int rect_width = map_x / (distance > 0 ? distance : 1);
    int rect_height = map_x / (distance > 0 ? distance : 1);
    
        for (int i = 0; i < map_x; i++) {
            for (int j = 0; j < map_y; j++) {
                if (i < rect_height && j < rect_width) {
                    cout << '*';
                }
                else {
                    cout << ' ';
                }
            }
            cout << endl;
        }
    }

}

int main() {
    char map[map_x][map_y];
    int player_pos[2] = { 0, 0 };
    char player_char = '^';

    while (true) {
        system("CLS");

        map_render(player_pos, map_x, map_y, map, player_char);

        switch (_getch()) {
        case 'w':
            if (player_pos[0] > 0 && map[player_pos[0] - 1][player_pos[1]] != '#') player_pos[0]--;
            break;
        case 'a':
            if (player_pos[1] > 0 && map[player_pos[0]][player_pos[1] - 1] != '#') player_pos[1]--;
            break;
        case 's':
            if (player_pos[0] < map_x - 1 && map[player_pos[0] + 1][player_pos[1]] != '#') player_pos[0]++;
            break;
        case 'd':
            if (player_pos[1] < map_y - 1 && map[player_pos[0]][player_pos[1] + 1] != '#') player_pos[1]++;
            break;
        default:
            break;
        }
    }
}
