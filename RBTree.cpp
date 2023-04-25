#include "RBTree.h"
#include <vector>
#include <string>
using std::vector;
using std::string;
using std::pair;
using std::to_string;

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

// Inserts a new node with the passed-in data.
void RBTree::insert(string word, int len, string pos, string def) {
    this->root = insertHelper(this->root, nullptr, word, len, pos, def);
}

// Recursively descends tree to insert node with passed-in data, then calls balanceSubtree() for
// each ancestor. Ultimately returns ptr to root (which may have been updated).
RBNode* RBTree::insertHelper(RBNode* node, RBNode* parent, string word, int len, string pos, string def) {
    if (node == nullptr) {
        if (parent == nullptr) {
            node = new RBNode(true, word, len, pos, def);
        }
        else {
            node = new RBNode(false, word, len, pos, def);
        }
    }
    if (word < node->getWord()) {
        node->left = insertHelper(node->left, node, word, len, pos, def);
    }
    else if (word > node->getWord()) {
        node->right = insertHelper(node->right, node, word, len, pos, def);
    }

    node = balanceSubtree(node); // If node is not a grandparent, nothing happens.

    if (node->isRoot) { // After rebalancing, always ensure that root is BLACK!
        node->isBlack = true;
    }

    return node;
}

// Checks passed-in node's grandchildren for a red-red conflict. If found, resolve conflict.
RBNode* RBTree::balanceSubtree(RBNode* grandpa) {
    // 1. Determine if red-red conflict exists. If so, continue. If not, return.
    bool foundConflict = false;
    RBNode* uncle = nullptr;
    RBNode* parent = nullptr;
    RBNode* node = nullptr;
    
    if (grandpa->left != nullptr) {
        if (hasConflict(grandpa->left, grandpa->left->left)) {
            uncle = grandpa->right;
            parent = grandpa->left;
            node = grandpa->left->left;
            foundConflict = true;
        }
        else if (hasConflict(grandpa->left, grandpa->left->right)) {
            uncle = grandpa->right;
            parent = grandpa->left;
            node = grandpa->left->right;
            foundConflict = true;
        }
    }
    if (grandpa->right != nullptr) {
        if (hasConflict(grandpa->right, grandpa->right->left)) {
            uncle = grandpa->left;
            parent = grandpa->right;
            node = grandpa->right->left;
            foundConflict = true;
        }
        else if (hasConflict(grandpa->right, grandpa->right->right)) {
            uncle = grandpa->left;
            parent = grandpa->right;
            node = grandpa->right->right;
            foundConflict = true;
        }
    }
    if (!foundConflict) { // Either "grandpa" is not a grandpa, or no red-red conflict detected.
        return grandpa;
    }
    
    // 2. If uncle is red, FLIP COLORS!
    if (uncle != nullptr && !uncle->isBlack) {
        grandpa->isBlack = false;
        uncle->isBlack = true;
        parent->isBlack = true;
    }
    // 3. If uncle is null or black, ROTATE!
    else {
        if (grandpa->left == parent) {
            if (parent->left == node) { // Left-Left Case.
                if (grandpa->isRoot) { // If grandpa is root, root will change after rotation!
                    grandpa->isRoot = false;
                    parent->isRoot = true;
                }
                grandpa = rotateRight(grandpa);
            }
            else if (parent->right == node) { // Left-Right Case.
                if (grandpa->isRoot) {
                    grandpa->isRoot = false;
                    node->isRoot = true;
                }
                grandpa = rotateLeftRight(grandpa);
            }
        }
        else if (grandpa->right == parent) { // Right-Left Case.
            if (parent->left == node) {
                if (grandpa->isRoot) {
                    grandpa->isRoot = false;
                    node->isRoot = true;
                }
                grandpa = rotateRightLeft(grandpa);
            }
            else if (parent->right == node) { // Right-Right Case.
                if (grandpa->isRoot) {
                    grandpa->isRoot = false;
                    parent->isRoot = true;
                }
                grandpa = rotateLeft(grandpa);
            }
        }

        // Fix colors after rotation.
        grandpa->isBlack = true;
        grandpa->left->isBlack = false;
        grandpa->right->isBlack = false;
    }

    return grandpa;
}

// Given a valid node and one of its children, returns true if there is a red-red conflict.
bool RBTree::hasConflict(RBNode* parent, RBNode* node) {
    if (node == nullptr) {
        return false;
    }
    else if (!parent->isBlack && !node->isBlack) {
        return true;
    }
    else {
        return false;
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

// Left-Left Case: Right rotation.
RBNode* RBTree::rotateRight(RBNode* node) {
    RBNode* grandchild = node->left->right;
    RBNode* newParent = node->left;
    newParent->right = node;
    node->left = grandchild;
    return newParent;
}

// Left-Right Case: Left-Right rotation.
RBNode* RBTree::rotateLeftRight(RBNode* node) {
    node->left = rotateLeft(node->left);
    RBNode* newParent = rotateRight(node);
    return newParent;
}

// Right-Left Case: Right-Left rotation.
RBNode* RBTree::rotateRightLeft(RBNode* node) {
    node->right = rotateRight(node->right);
    RBNode* newParent = rotateLeft(node);
    return newParent;
}

// Searches tree for the passed-in word. If found, returns vec of word, length (as a string),
// POS, and def. Else, returns empty vec.
vector<string> RBTree::search(string word) const {
    RBNode* found = searchHelper(this->root, word);
    if (found != nullptr) {
        return { found->getWord(), to_string(found->getLen()), found->getPOS(), found->getDef() };
    }
    else {
        return {};
    }
}

// Recursively descends the tree to find matching word. Ultimately returns ptr to node if
// successful, and nullptr otherwise.
RBNode* RBTree::searchHelper(RBNode* node, string word) const {
    if (node == nullptr) {
        return nullptr;
    }
    if (word < node->getWord()) {
        return searchHelper(node->left, word);
    }
    else if (word > node->getWord()) {
        return searchHelper(node->right, word);
    }
    else { // Found word.
        return node;
    }
}

// Testing: Performs inorder traversal. Returns a vec of <words, and their node colors>.
vector<pair<string, bool>> RBTree::traverse() const {
    vector<pair<string, bool>> vec;
    traverseHelper(this->root, vec);
    return vec;
}

// Testing: Performs inorder traversal, pushing each word and node color into the vec.
void RBTree::traverseHelper(RBNode* node, vector<pair<string, bool>>& vec) const {
    if (node != nullptr) {
        traverseHelper(node->left, vec);
        vec.push_back(std::make_pair(node->getWord(), node->isBlack));
        traverseHelper(node->right, vec);
    }
}
