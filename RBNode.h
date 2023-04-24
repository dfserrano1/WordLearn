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
        bool isBlack;

        RBNode* left;
        RBNode* right;
        RBNode* grandpa;
        RBNode* parent;
        RBNode* uncle;

        string getWord() const { return word; }
        int getLen() const { return len; }
        string getPOS() const { return pos; }
        string getDef() const { return def; }

        void setExtendedFam();

        RBNode();
        RBNode(RBNode* parent, string word, int length, string pos, string def);
};
