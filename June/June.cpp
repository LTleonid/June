#include <iostream>
using namespace std;
int main() {
	
	char map[5][5];
	int pos[2]{ 2,2 };
	for (int i = 0; i < 5; i++) {
		for (int j = 0; j < 5; j++) {
			if (i == pos[0] && j == pos[1]) {
				map[i][j] = '*';
			}
			else {
				map[i][j] = '#';
			}
			cout << map[i][j];
		}
		cout << endl;
	}

}