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

using namespace std;

const int width = 64;
double delta = 0.2;
double rho = 0.6;
vector<vector<int>> grid(width, vector<int>(width, 0));

vector<double> init_values = {0, -1, 1};
vector<double> init_weights = {1-rho, rho/2, rho/2};
vector<double> weights = {(1-delta)/8, 1/8, (1+delta)/8, (1-delta)/8, 0, (1+delta)/8, (1-delta)/8, 1/8, (1+delta)/8};

random_device rd;
mt19937 gen(rd());
discrete_distribution init_dist(init_weights.begin(), init_weights.end());

void display(vector<vector<int>>& grid) {
    for (const auto& row : grid) {
        for (int val : row) {
            switch (val) {
                case -1: cout << RED << "O" << RESET << " "; break;
                case 0:  cout << "  "; break;
                case 1:  cout << GREEN << "O" << RESET << " "; break;
                default: cout << "? "; break;
            }
        }
        cout << endl;
    }
    cout << endl;
}

int main(int argc, char* argv[]) { // example usage: ./nesm 100
    
    // // mixed
    // for (int i = 0; i < width; i++) {
    //     for (int j = 0; j < width; j++) {
    //         int idx = init_dist(gen);
    //         grid[i][j] = init_values[idx];
    //     }
    // }
    
    // unmixed
    for (int i = 0; i < width / 2; i++) {
        for (int j = 0; j < width; j++) {
            int idx = init_dist(gen);
            grid[i][j] = abs(init_values[idx]);
        }
    }
    for (int i = width / 2; i < width; i++) {
        for (int j = 0; j < width; j++) {
            int idx = init_dist(gen);
            grid[i][j] = -1 * abs(init_values[idx]);
        }
    }

    display(grid);
    
    int num_iter = 100;
    if (argc >= 2) {
        num_iter = atoi(argv[1]);
    }
    
    for (int n = 0; n < num_iter; n++) {
        for (int i = 0; i < width; i++) {
            for (int j = 0; j < width; j++) {
                int val = grid[i][j];
                if (val == 0) continue;
                vector<double> wts = weights;
                int count = 0;
                for (int p = i-1+width; p <= i+1+width; p++) {
                    for (int q = j-1+width; q <= j+1+width; q++) {
                        wts[count] *= (grid[p % width][q % width] == 0);
                        count++;
                    }
                }
                double sum = 0.0;
                for (double d : wts) {
                    sum += d;
                }
                if (sum == 0.0) continue;
                for (int i = 0; i < wts.size(); i++) {
                    wts[i] /= sum;
                }
                discrete_distribution dist(wts.begin(), wts.end());
                int idx = dist(gen);
                grid[i][j] = 0;
                assert(grid[(i + (idx >= 3) + (idx >= 6) - 1 + width) % width][(j + (idx % 3) - 1 + width) % width] == 0);
                grid[(i + (idx >= 3) + (idx >= 6) - 1 + width) % width][(j + (idx % 3) - 1 + width) % width] = val;
            }
        }
    }

    display(grid);
    
    return 0;
}