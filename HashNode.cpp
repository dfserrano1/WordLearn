#include "HashNode.h"

// Default Constructor (not used).
HashNode::HashNode() {
    word = "";
    len = 0;
    pos = "";
    def = "";
    next = nullptr;
}

// Parameterized Constructor.
HashNode::HashNode(string word, int len, string pos, string def) {
    this->word = word;
    this->len = len;
    this->pos = pos;
    this->def = def;
    this->next = nullptr;
}
