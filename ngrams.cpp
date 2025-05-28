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

using namespace std;

string srcPath = "src.txt";
string destPath = "dest.txt";
int n = 2;
int maxWords = 100;
int maxStrSize = 10;

vector<char> alphabet = {'a','b','c','d','e','f','g','h','i','j','k','l','m','n','o','p','q','r','s','t','u','v','w','x','y','z'};
std::vector<double> frequencies = {8.2, 1.5, 2.8, 4.3, 12.7, 2.2, 2.0, 6.1, 7.0, 0.15, 0.77, 4.0, 2.4,
                                    6.7, 7.5, 1.9, 0.095, 6.0, 6.3, 9.1, 2.8, 1.0, 2.4, 0.15, 2.0, 0.07};

random_device rd;
mt19937 gen(rd());
discrete_distribution<> dis(frequencies.begin(), frequencies.end());

string trim(const string& str) {
    const auto strBegin = str.find_first_not_of(" \t\r\n");
    if (strBegin == string::npos) return ""; // no content

    const auto strEnd = str.find_last_not_of(" \t\r\n");
    return str.substr(strBegin, strEnd - strBegin + 1);
}

bool create_words(int n, string src, string dest) {
    ifstream infile(src);
    if (!infile.is_open()) {
        cerr << "Error: Unable to open file: " << src << endl;
        return false;
    }
    ofstream outfile(dest);
    if (!outfile.is_open()) {
        cerr << "Error: Cannot create or write to file: " << dest << endl;
        return false;
    }
    
    vector<char> processedChars;
    char ch;
    char prev = ' ';
    while (infile.get(ch)) {
        if (isalpha(ch)) {
            prev = tolower(ch);
            processedChars.push_back(prev);
        } else if (prev != ' ') {
            prev = ' ';
            processedChars.push_back(prev);
        }
    }
    infile.close();

    int sz = processedChars.size();
    vector<string> ngrams;
    for (int i = 0; i < sz-n; i++) {
        string ngram = "";
        for (int j = i; j <= i+n; j++) {
            if (processedChars[j] == ' ') {
                break;
            }
            ngram.append(1, processedChars[j]);
        }
        if (ngram.length() == n+1) ngrams.push_back(ngram);
    }
    sort(ngrams.begin(), ngrams.end());
    
    for (int i = 0; i < ngrams.size(); i++) {
        cout << ngrams[i] << " ";
        if (i % 10 == 9) cout << endl;
    }
    cout << endl;

    unordered_map<string, unordered_map<char, int>> counts;
    for (const string& word : ngrams) {
        string prefix = word.substr(0, word.size()-1);
        char suffix = word.back();
        counts[prefix][suffix]++;
    }
    unordered_map<string, unordered_map<char, double>> probMap;
    for (const auto& [prefix, suffixMap] : counts) {
        int total = 0;
        for (const auto& [ch, count] : suffixMap) {
            total += count;
        }
        for (const auto& [ch, count] : suffixMap) {
            probMap[prefix][ch] = static_cast<double>(count) / total;
        }
    }

    vector<string> keys;
    for (const auto& pair : probMap) {
        keys.push_back(pair.first);
    }
    uniform_int_distribution<> key_dis(0, keys.size()-1);

    int w = 0;
    for (; w < maxWords; w++) {
        int strSize = n;
        string curr = keys[key_dis(gen)];
        for (char c : curr) {
            outfile.put(c);
        }
        while ((probMap.find(curr) != probMap.end()) && strSize < maxStrSize) {
            vector<char> chars;
            vector<double> weights;
            for (const auto& [ch, prob] : probMap[curr]) {
                chars.push_back(ch);
                weights.push_back(prob);
            }
            discrete_distribution<> dist(weights.begin(), weights.end());
            int index = dist(gen);
            char nextChar = chars[index];
            curr.erase(0, 1);
            curr.append(1, nextChar);
            outfile.put(curr[n-1]);
            strSize++;
        }
        ((w % 10) == 9) ? outfile.put('\n') : outfile.put(' ');
    }

    return w>0;
}

int main(int argc, char* argv[]) { // example usage: ./ngrams 2 src.txt dest.txt
    if (argc >= 2) {
        n = max(atoi(argv[1]),1);
        if (argc >= 3) {
            srcPath = argv[2];
            if (argc >= 4) {
                destPath = argv[3];
            }
        }
    }

    if (!create_words(n, srcPath, destPath)) {
        cerr << "Error creating words." << endl;
        return 1;
    }
    
    return 0;
}