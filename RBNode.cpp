#include "RBNode.h"

// Default Constructor (not used).
RBNode::RBNode() {
    word = "";
    len = 0;
    pos = "";
    def = "";
    isRoot = false;
    isBlack = false;
    left = nullptr;
    right = nullptr;
}

// Parameterized Constructor.
RBNode::RBNode(bool isRoot, string word, int len, string pos, string def) {
    this->word = word;
    this->len = len;
    this->pos = pos;
    this->def = def;
    this->isRoot = isRoot;
    this->isBlack = false; // New RB nodes start as RED by default.
    this->left = nullptr;
    this->right = nullptr;
}
