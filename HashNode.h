#pragma once
#include <string>
using std::string;

class HashNode {
    private:
        string word;
        int len;
        string pos;
        string def;
    public:
        HashNode* next;

        string getWord() const { return word; }
        int getLen() const { return len; }
        string getPOS() const { return pos; }
        string getDef() const { return def; }

        HashNode();
        HashNode(string word, int len, string pos, string def);
};
