#include <iostream>
#include <iomanip>
#include <string>
#include <sstream>
#include <fstream>
#include <ostream>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <cstring>
#include <vector>
#include <unordered_set>
#include <algorithm>
#include <cassert>
#include <random>
#include <cctype>
#include <unordered_map>

#define RESET   "\033[0m"
#define RED     "\033[31m"
#define GREEN   "\033[32m"
#define YELLOW  "\033[33m"
#define GREY    "\033[90m"
#define BROWN   "\x1b[38;5;94m"
#define BLUE    "\x1b[38;5;57m"
#define WHITE   "\x1b[38;5;254m"

using namespace std;

enum Terrain {
    EMPTY = 0,
    ROCK = 1,
    DIRT = 2,
    GRASS = 3,
    SNOW = 4,
    WATER = 5
};

const int width = 64;
const int height = 64;

vector<vector<int>> grid(width, vector<int>(height, DIRT));
vector<vector<int>> grid_next(width, vector<int>(height, EMPTY));
vector<vector<int>> heights(width, vector<int>(height, 0));
// vector<vector<int>>> grid_layers;

void display(vector<vector<int>>& grid) {
    for (const auto& row : grid) {
        for (int val : row) {
            switch (val) {
                case 0:  cout << "  "; break;
                case 1:  cout << GREY << "O" << RESET << " "; break;
                // case 2:  cout << BROWN << "O" << RESET << " "; break;
                case 2:  cout << "  "; break;
                case 3:  cout << GREEN << "O" << RESET << " "; break;
                case 4:  cout << WHITE << "O" << RESET << " "; break;
                case 5:  cout << BLUE << "O" << RESET << " "; break;
                default: cout << "X "; break;
            }
        }
        cout << endl;
    }
    cout << endl;
}

int main(int argc, char* argv[]) { // example usage: ./terrain 100
    
    int num_iter = 100;
    if (argc >= 2) {
        num_iter = atoi(argv[1]);
    }
    
    for (int i = 0; i < width; i++) {
        for (int j = 0; j < width; j++) {
            if (rand() % 100 < 5) {
                grid[i][j] = GRASS;
            }
        }
    }

    display(grid);
    
    for (int n = 0; n < num_iter; n++) {
        grid_next = grid;
        for (int i = 0; i < width; i++) {
            for (int j = 0; j < width; j++) {
                int curr = grid[i][j];
                int ngrass = 0;
                int ndirt = 0;
                for (int dx = -1; dx <= 1; dx++) {
                    for (int dy = -1; dy <= 1; dy++) {
                        int ni = (i + dx + width) % width;
                        int nj = (j + dx + width) % width;
                        if (grid[ni][nj] == GRASS) ngrass++;
                        if (grid[ni][nj] == DIRT) ndirt++;
                    }
                }
                if (curr == DIRT && ngrass >= 3 && rand() % 100 < 20) {
                    grid_next[i][j] = GRASS;
                } else if (curr == GRASS && ndirt >= 2 && rand() % 100 < 10) {
                    grid_next[i][j] = DIRT;
                } else {
                    grid_next[i][j] = grid[i][j];
                }
            }
        }
        swap(grid, grid_next);
    }

    display(grid);
    
    return 0;
}