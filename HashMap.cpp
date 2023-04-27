#include "HashMap.h"
#include <vector>
#include <string>
#include <random>
#include <ctime>
using std::vector;
using std::string;
using std::to_string;

// Default Constructor.
HashMap::HashMap() {
    entryCount = 0;
    loadFactor = 0;
    maxLoadFactor = 0.8;
    table.resize(5);
}

// Destructor.
HashMap::~HashMap() {
    for (unsigned int i = 0; i < this->table.size(); i++) { // Iterate through buckets.
        while (this->table.at(i) != nullptr) { // If bucket is occupied, delete all nodes in it.
            HashNode* nodeToDelete = this->table.at(i);
            this->table.at(i) = this->table.at(i)->next;

            delete nodeToDelete;
            this->entryCount--;
        }
        if (this->entryCount == 0) { // Found all nodes, so no need to keep searching.
            break;
        }
    }
}

// Converts the passed-in word into a reduced hash code.
unsigned int HashMap::hash(string word) const {
    unsigned int hashCode = 0;

    for (char c : word) { // Finds sum of ASCII characters.
        hashCode += (unsigned int)c;
    }

    hashCode = hashCode % this->table.size(); // Reduce hash code to fit in table.
    return hashCode;
}

// Inserts word into HashMap.
void HashMap::insert(string word, int len, string pos, string def) {
    // 1. Get hash code.
    unsigned int hashCode = hash(word);

    // 2. Insert.
    HashNode* newNode = nullptr;

    if (this->table.at(hashCode) == nullptr) { // Bucket is empty. Insert new node at head.
        newNode = new HashNode(word, len, pos, def);
        this->table.at(hashCode) = newNode;
    }
    else { // Bucket is occupied. Use Separate Chaining!
        HashNode* curr = this->table.at(hashCode);

        while (curr->next != nullptr) { // Traverse linked list to the end.
            if (curr->getWord() == word) { // Stop! Word already exists in Hash Map.
                return;
            }
            curr = curr->next;
        }
        if (curr->getWord() == word) { // In case while loop was not entered, check again.
            return;
        }

        newNode = new HashNode(word, len, pos, def);
        curr->next = newNode; // Insert new node at the end.
    }

    if (len == 7) { // If newly inserted node was 7 letters, push to vec.
        this->sevenLetterNodes.push_back(newNode);
    }

    // 3. Update loadFactor.
    this->entryCount++;
    this->loadFactor = ((double)this->entryCount) / this->table.size();

    // 4. Rehash if needed.
    if (this->loadFactor >= this->maxLoadFactor) { // loadFactor has reached max.
        rehash();
    }
}

// Performs rehashing procedure after insert() caused loadFactor to reach max.
void HashMap::rehash() {
    // 1. Resize table.
    this->table.resize(this->table.size() * 2); // Double the size of the table.

    // 2. Remove everything from table.
    vector<HashNode*> vecOfNodes; // Transfer all nodes to this vec instead.

    for (unsigned int i = 0; i < this->table.size(); i++) { // Iterate through buckets.
        while (this->table.at(i) != nullptr) { // If bucket is occupied, remove all nodes in it.
            HashNode* nodeToPush = this->table.at(i);
            this->table.at(i) = this->table.at(i)->next;

            vecOfNodes.push_back(nodeToPush); // Push all nodes to the vec.
            this->entryCount--;
        }
        if (this->entryCount == 0) { // Found all nodes, so no need to keep searching.
            break;
        }
    }
    this->sevenLetterNodes.clear();

    // 3. Rehash and reinsert everything into table.
    for (HashNode* node : vecOfNodes) { // entryCount and loadFactor are currently 0.
        insert(node->getWord(), node->getLen(), node->getPOS(), node->getDef());
        delete node;
    }
}

// Gets a random 7-letter word from the table. Returns vec containing the word, its lenth (as a
// string), its POS, and its def.
vector<string> HashMap::getTarget() const {
    if (sevenLetterNodes.empty()) {
        return {};
    }

    std::mt19937 rng(std::time(0)); // Seed rng with current time.
    std::uniform_int_distribution<int> dist(0, sevenLetterNodes.size() - 1);
    unsigned int randomIndex = dist(rng);

    HashNode* target = sevenLetterNodes.at(randomIndex);
    return {target->getWord(), to_string(target->getLen()), target->getPOS(), target->getDef()};
}

// Searches bucket for the passed-in word. If found, returns vec of word, length (as a string),
// POS, and def. Else, returns empty vec.
vector<string> HashMap::search(string word) const {
    unsigned int hashCode = hash(word);

    if (this->table.at(hashCode) == nullptr) { // Bucket is empty.
        return {};
    }
    else { // Bucket is occupied.
        HashNode* curr = this->table.at(hashCode);

        while (curr != nullptr) { // Traverse linked list to the end.
            if (curr->getWord() == word) { // Found word!
                return {curr->getWord(), to_string(curr->getLen()), curr->getPOS(), curr->getDef()};
            }
            curr = curr->next;
        }
    }
    return {}; // Word was not found in bucket.
}
