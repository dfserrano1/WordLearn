#pragma once
#include "HashNode.h"
#include <vector>
#include <string>
using std::vector;
using std::string;

class HashMap {
    private:
        vector<HashNode*> table;
        vector<HashNode*> sevenLetterNodes;

        unsigned int entryCount;
        double loadFactor;
        double maxLoadFactor;

        unsigned int hash(string word) const;
        void rehash();
    public:
        HashMap();
        ~HashMap();
        void insert(string word, int len, string pos, string def);
        vector<string> getTarget() const;
        vector<string> search(string word) const;
};
