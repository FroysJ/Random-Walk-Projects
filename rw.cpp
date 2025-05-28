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

using namespace std;

int T = 10; // finite time horizon
int x = 0; // starting point X_0
vector<int> X; // RVs X_0, X_1, ... X_T (X.size() is T+1)
vector<int> step_sizes = {-1,1}; // possible step sizes
vector<double> step_probs = {0.5,0.5}; // probabilities of step sizes

random_device rd;
mt19937 gen(rd());
discrete_distribution<> dist(step_probs.begin(), step_probs.end());

int main() {
    X.push_back(x);
    
    for (int i = 1; i <= T; i++) {
        int idx = dist(gen);
        x += step_sizes[idx];
        X.push_back(x);
    }
    
    for (int i = 0; i <= T; i++) {
        cout << "X_{" << i << "} = " << X[i] << endl;
    }
    
    return 0;
}
