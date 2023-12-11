#pragma once
#include <string.h>
#include <iostream>
#include <unordered_map>
#include <fstream>
using namespace std;

class HuffmanUtility {
public:
    void writeHeader(ofstream& outputStream, unordered_map<char, string> codeMap);
    unordered_map<char, string> readHeader(ifstream& inputStream);
};