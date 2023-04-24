#pragma once
#include "RBNode.h"
#include <vector>
#include <string>
using std::vector;
using std::string;

// Red-Black Tree Properties:
// 1. A node is either RED or BLACK.
// 2. Root is always BLACK.
// 3. RED node always has BLACK children.
// 4. Number of BLACK nodes in any path from root to leaf is the same.
// 5. Nullptrs are considered BLACK.

class RBTree {
    private:
        RBNode* root;
        void clearTree(RBNode* node);
        RBNode* insertHelper(RBNode* node, RBNode* parent,
                            string word, int len, string pos, string def);
        RBNode* balanceSubtree(RBNode* node);
        RBNode* rotateLeft(RBNode* node);
        RBNode* rotateRight(RBNode* node);
        RBNode* rotateLeftRight(RBNode* node);
        RBNode* rotateRightLeft(RBNode* node);
        RBNode* searchHelper(RBNode* node, string word);
    public:
        RBTree() : root(nullptr) {}
        ~RBTree();
        void insert(string word, int len, string pos, string def);
        vector<string> search(string word);
};
