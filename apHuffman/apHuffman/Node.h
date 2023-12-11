#pragma once
#include <iostream>
#include <string>
using std::string;
using std::istream;
using std::ostream;

class Node {
    char character;
    int frequency;
public:
    int getFrequency() const;

private:
    Node* left, * right;
public:
    Node(char character, int frequency);
    Node(char character);

    Node* getLeft() const;

    Node* getRight() const;

    void setLeft(Node* left);

    void setRight(Node* right);

    char getCharacter() const;
};