#include "RBTree.h"
#include <vector>
#include <string>
using std::vector;
using std::string;

// Destructor.
RBTree::~RBTree() {
    clearTree(this->root);
}

// Recursively deletes each node using postorder traversal (LRN).
void RBTree::clearTree(RBNode* node) {
    if (node != nullptr) {
        clearTree(node->left);
        clearTree(node->right);
        delete node;
    }
}

void RBTree::insert(string word, int len, string pos, string def) {
    insertHelper(this->root, nullptr, word, len, pos, def);
}

RBNode* RBTree::insertHelper(RBNode* node, RBNode* parent,
                            string word, int len, string pos, string def) {
    if (node == nullptr) {
        node = new RBNode(parent, word, len, pos, def);
        node->setExtendedFam();
    }
    if (word < node->getWord()) {
        node->left = insertHelper(node->left, node, word, len, pos, def);
    }
    else if (word > node->getWord()) {
        node->right = insertHelper(node->right, node, word, len, pos, def);
    }

    node = balanceSubtree(node);

    return node;
}

RBNode* RBTree::balanceSubtree(RBNode* node) {
    if (node->isBlack) { // Skip over black nodes as we ascend the tree recursively.
        return node;
    }
    else { // Node is RED.
        if (node == this->root) { // Root must always be BLACK.
            node->isBlack = true;
            return node;
        }
        else if (node->parent->isBlack) { // If parent is BLACK, no conflict.
            return node;
        }
        else { // Red-red conflict.
            RBNode* parent = node->parent;
            RBNode* grandpa = node->grandpa;
            RBNode* uncle = node->uncle;

            if (uncle != nullptr && !uncle->isBlack) { // If uncle is red, FLIP COLORS.
                parent->isBlack = true;
                uncle->isBlack = true;
                grandpa->isBlack = false;
                //
                return node;
            }
            else { // If uncle is null or black, ROTATE.
                if (node == parent->right && parent == grandpa->right) {
                    
                }
            }
        }
    }
}

// Right-Right Case: Left rotation.
RBNode* RBTree::rotateLeft(RBNode* node) {
    RBNode* grandchild = node->right->left;
    RBNode* newParent = node->right;
    newParent->left = node;
    node->right = grandchild;
    return newParent;
}

// Searches tree for the passed-in word. If found, returns vec of word, POS, and def. Else,
// returns empty vec.
vector<string> RBTree::search(string word) {
    RBNode* found = searchHelper(this->root, word);
    if (found != nullptr) {
        return { found->getWord(), found->getPOS(), found->getDef() };
    }
    else {
        return {};
    }
}
