#pragma once
#include <string>
using std::string;

class RBNode {
    private:
        string word;
        int len; // Length.
        string pos; // Part of Speech.
        string def; // Definition.
    public:
        bool isRoot;
        bool isBlack;
        RBNode* left;
        RBNode* right;

        string getWord() const { return word; }
        int getLen() const { return len; }
        string getPOS() const { return pos; }
        string getDef() const { return def; }

        RBNode();
        RBNode(bool isRoot, string word, int len, string pos, string def);
};
