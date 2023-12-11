#include "Huffman.h"
#include <queue>
#include <bitset>
#include <string>
#include <iostream>
#include <fstream>

using namespace std;

#define INTERNAL_NODE_CHARACTER char(128)
#define PSEUDO_EOF char(129)
#define CHARACTER_CODE_SEPERATOR char(130)
#define HEADER_ENTRY_SEPERATOR char(131)
#define HEADER_TEXT_SEPERATOR char(132)

int Huffman::myCompartor::operator()(Node* node1, Node* node2) {
    return node1->getFrequency() > node2->getFrequency();
}

void Huffman::huffer(unordered_map<char, int> frequencyMap) {
    priority_queue <Node*, vector<Node*>, myCompartor> HufferQueue;
    string tempString;
    Node* leftNode, * rightNode, * newNode;
    for (const auto& item : frequencyMap)
        HufferQueue.push(new Node(item.first, item.second));
    HufferQueue.push(new Node(PSEUDO_EOF, 1));
    while (HufferQueue.size() != 1) {
        leftNode = HufferQueue.top();
        HufferQueue.pop();
        rightNode = HufferQueue.top();
        HufferQueue.pop();
        newNode = new Node(INTERNAL_NODE_CHARACTER, leftNode->getFrequency() + rightNode->getFrequency());
        HufferQueue.push(newNode);
        newNode->setLeft(leftNode);
        newNode->setRight(rightNode);
    }
    encodeCharacters(HufferQueue.top(), tempString);
}

void Huffman::encodeCharacters(Node* rootNode, string codeString) {

    if (!rootNode)
        return;
    if (rootNode->getCharacter() != INTERNAL_NODE_CHARACTER) {
        codeMap[rootNode->getCharacter()] = codeString;

    }
    encodeCharacters(rootNode->getLeft(), codeString + "0");
    encodeCharacters(rootNode->getRight(), codeString + "1");
}

void Huffman::compressTofile(string InputfileName, string OutputfileName) {
    char character;
    string file;
    ifstream inputStream(InputfileName, ios::in);
    ofstream outputStream(OutputfileName, ios::out | ios::binary);

    // Verifica se os arquivos foram abertos corretamente
    if (!inputStream.is_open() || !outputStream.is_open()) {
        cerr << "Erro ao abrir o arquivo de entrada ou saída." << endl;
        return;
    }

    // Escreve o cabeçalho no arquivo de saída
    writeHeader(outputStream);

    // Lê o arquivo de entrada e constrói a string de bits
    while (inputStream.get(character)) {
        if (codeMap.find(character) != codeMap.end()) {
            file += codeMap[character];
        }
        else {
            cerr << "Erro: caractere não encontrado no mapa de códigos." << endl;
            return;
        }
    }

    // Adiciona o código de fim de arquivo
    file += codeMap[PSEUDO_EOF];

    // Adiciona zeros para garantir um número completo de bytes
    unsigned long remainder = file.size() % 8;
    if (remainder != 0) {
        file += string(8 - remainder, '0');
    }

    // Converte a string de bits para bytes e escreve no arquivo de saída
    for (size_t i = 0; i < file.size(); i += 8) {
        bitset<8> bits(file.substr(i, 8));
        outputStream.put(static_cast<char>(bits.to_ulong()));
    }

    inputStream.close();
    outputStream.close();
}


void Huffman::writeHeader(ofstream& outputStream) {
    for (const auto& item : codeMap)
        outputStream << item.first << CHARACTER_CODE_SEPERATOR << item.second << HEADER_ENTRY_SEPERATOR;
    outputStream << HEADER_TEXT_SEPERATOR;
}

void Huffman::deHuffer(string compressedFileName, string decompressedFileName) {
    char character;
    string codeString;
    ifstream inputStream;
    inputStream.open(compressedFileName, ios::in);
    readHeader(inputStream);
    while (inputStream.get(character)) {
        bitset<8> bits(character);
        codeString += bits.to_string();


    }
    Node* rootNode = buildDecodingTree(codeMap);
    decompressToFile(codeString, rootNode, decompressedFileName);


}

void Huffman::readHeader(ifstream& inputStream) {
    codeMap.clear();
    char character;
    inputStream.get(character);
    char key = character;
    while (character != HEADER_TEXT_SEPERATOR) {
        if (character == CHARACTER_CODE_SEPERATOR) {
            inputStream.get(character);
            while (character != HEADER_ENTRY_SEPERATOR) {
                codeMap[key] += character;
                inputStream.get(character);
            }
        }
        else
            key = character;
        inputStream.get(character);
    }
}

Node* Huffman::buildDecodingTree(unordered_map<char, string> encodingMap) {

    Node* rootNode = new Node(INTERNAL_NODE_CHARACTER);
    Node* previousNode;

    for (const auto& item : encodingMap) {
        previousNode = rootNode;
        Node* newNode = new Node(item.first);
        string characterCode = item.second;
        for (int i = 0; i < characterCode.size(); ++i) {
            if (characterCode[i] == '0') {
                if (i == characterCode.size() - 1)
                    previousNode->setLeft(newNode);
                else {
                    if (!previousNode->getLeft()) {
                        previousNode->setLeft(new Node(INTERNAL_NODE_CHARACTER));
                        previousNode = previousNode->getLeft();
                    }
                    else previousNode = previousNode->getLeft();
                }
            }
            else {
                if (i == characterCode.size() - 1)
                    previousNode->setRight(newNode);
                else {
                    if (!previousNode->getRight()) {
                        previousNode->setRight(new Node(INTERNAL_NODE_CHARACTER));
                        previousNode = previousNode->getRight();
                    }
                    else previousNode = previousNode->getRight();
                }
            }
        }

    }
    return rootNode;
}

void Huffman::decompressToFile(string codeString, Node* rootNode, string decompressedFileName) {
    ofstream outputStream;
    outputStream.open(decompressedFileName, ios::out);
    Node* traversingPointer = rootNode;
    for (int i = 0; i < codeString.size() + 1; ++i) {
        if (codeString[i] == '0')
            traversingPointer = traversingPointer->getLeft();
        else
            traversingPointer = traversingPointer->getRight();

        if (traversingPointer->getCharacter() != INTERNAL_NODE_CHARACTER) {
            if (traversingPointer->getCharacter() == PSEUDO_EOF)
                break;
            outputStream << traversingPointer->getCharacter();
            traversingPointer = rootNode;
        }
    }
    outputStream.flush();
    outputStream.close();
}