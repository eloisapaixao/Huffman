#pragma once
#include <string.h>
#include <iostream>
#include <unordered_map>
#include <fstream>
#include <iostream>
#include "Node.h"
using namespace std;

class Huffman {

    unordered_map<char, string> codeMap;
    void encodeCharacters(Node* rootNode, string codeString);
    void writeHeader(ofstream& outputStream);
    void readHeader(ifstream& inputStream);
    Node* buildDecodingTree(unordered_map<char, string> encodingMap);
    class myCompartor {
    public:
        int operator() (Node* node1, Node* node2);
    };

public:
    void huffer(unordered_map<char, int> frequencyMap);
    void deHuffer(string fileName, string decompressedFileName);
    void compressTofile(string InputfileName, string OutputfileName);
    void decompressToFile(string codeString, Node* rootNode, string decompressedFileName);
};