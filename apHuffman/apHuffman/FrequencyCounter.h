#pragma once
#include <string.h>
#include <iostream>
#include <unordered_map>
#include <fstream>
using namespace std;

class FrequencyCounter {
    unordered_map<char, int> frequencyMap;

public:
    const unordered_map<char, int>& getFrequencyMap() const;
    void readFile(string fileName);


};