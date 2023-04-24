#include "RBNode.h"

// Default Constructor.
RBNode::RBNode() {
    parent = nullptr;
    word = "";
    len = 0;
    pos = "";
    def = "";
    isBlack = false;
    left = nullptr;
    right = nullptr;
}

// Parameterized Constructor.
RBNode::RBNode(RBNode* parent, string word, int len, string pos, string def) {
    this->parent = parent;
    this->word = word;
    this->len = len;
    this->pos = pos;
    this->def = def;
    this->isBlack = false; // New RB nodes start as RED by default.
    this->left = nullptr;
    this->right = nullptr;
}

void RBNode::setExtendedFam() {
    if (parent != nullptr && parent->parent != nullptr) { // If grandpa exists...
        grandpa = parent->parent; // Set grandpa.

        if (grandpa->left == parent) { // If parent is a left child...
            uncle = grandpa->right; // Set uncle as grandpa's right child (possibly nullptr).
        }
        else if (grandpa->right == parent) { // If parent is a right child...
            uncle = grandpa->left; // Set uncle as grandpa's left child (possibly nullptr).
        }
    }
    else {
        grandpa = nullptr;
        uncle = nullptr;
    }
}
