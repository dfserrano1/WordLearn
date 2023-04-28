#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <chrono>
#include "RBTree.h"
#include "HashMap.h"

using namespace std;

// Insert all data points into the tree using the RB Tree insert function.
double loadRBTree(RBTree &treeRB);
// Insert all data points into the tree using the hashmap insert function.
double loadHashmap(HashMap &hashMap);

// Create grid of 7 rows.
void generateGrid(vector<pair<string,string>> &grid);
// Print current grid showing game round record of input guesses and empty rows.
void printGrid(vector<pair<string,string>> &grid, int attempt);
// Replace grid with underscores and blank values.
void resetGrid(vector<pair<string,string>> &grid);

// Remove underscore characters from input in order to search in structures.
string removeUnderscores(string word);
// Validate input. Determine the following action.
int validateInput(string input, vector<pair<string,string>> &grid, int attempt, vector<string> targetWord);
// Returns a string of symbols representing the accuracy of a valid input guess.
string symbolResults(string targetWord, string input);

int main() {

    cout << "Welcome to WordLearn!" << endl;
    double difference;
    // Load Red-Black tree and output time elapsed.
    RBTree treeRB = RBTree();
    double loadingTime;
    loadingTime = loadRBTree(treeRB);
    cout << "Time taken to load Red-Black Tree: " << loadingTime << " seconds" << endl;
    difference = loadingTime;

    // Load hashmap and output time elapsed.
    HashMap hashMap = HashMap();
    loadingTime = 0;
    loadingTime = loadHashmap(hashMap);
    cout << "Time taken to load Hash Map: " << loadingTime << " seconds" << endl;
    difference = difference - loadingTime;
    difference = abs(difference);
    cout << "Difference: " << difference<< endl;

    bool game = true;
    bool round = false;
    int menu;
    int attempt = 0;

    // Will be used to store valid guess-word inputs. Will be printed to show game round record.
    // Grid: {1. [first(guess/blank), second(symbols)], 2. [], 3. [], ..., 8. []}
    vector<pair<string,string>> grid;
    generateGrid(grid);

    // Will be used to store a randomly generated 7-letter word from the dataset.
    // {target word, its length (as a string), its POS, its def}
    vector<string> targetWord;

    while (game) {

        // Print game menu.
        cout << "Game Menu: " << endl;
        cout << "1: Red-Black Tree Game" << endl;
        cout << "2. Hash Map Game" << endl;
        cout << "3. Rules" << endl;
        cout << "4: Quit" << endl;
        cout << "Enter 1-4 to make a menu selection" << endl;
        cin >> menu;

        attempt = 0;
        switch (menu) {
            case 1 : { // WordLearn game using Red-Black tree.
                // Find target word from RB tree.
                auto start = chrono::steady_clock::now();
                targetWord = treeRB.getTarget();
                auto end = chrono::steady_clock::now();
                double loadingTimeNS = double (chrono::duration_cast <chrono::nanoseconds> (end-start).count());
                loadingTime = loadingTimeNS/(1e9);
                cout << "Time taken to search for a target word: " << loadingTime << " seconds" << endl;
                if (targetWord.empty()){
                    cout << "ERROR: No 7-letter words found." << '\n' << endl;
                    continue;
                }

                // Print target word details.
                cout << "Target Length: " << targetWord[1] << endl;
                cout << "Target Part of Speech: " << targetWord[2] << endl;
                cout << "Target Definition: " << targetWord[3] << '\n' << endl;

                round = true;
                break;
            }
            case 2: { // WordLearn game using Hash Map.
                // Find target word from Hash Map.
                auto start = chrono::steady_clock::now();
                targetWord = hashMap.getTarget();
                auto end = chrono::steady_clock::now();
                double loadingTimeNS = double (chrono::duration_cast <chrono::nanoseconds> (end-start).count());
                loadingTime = loadingTimeNS/(1e9);
                cout << "Time taken to search for a target word: " << loadingTime << " seconds" << endl;
                if (targetWord.empty()) {
                    cout << "ERROR: No 7-letter words found." << '\n' << endl;
                    continue;
                }
                // Print target word details.
                cout << "Target Length: " << targetWord[1] << endl;
                cout << "Target Part of Speech: " << targetWord[2] << endl;
                cout << "Target Definition: " << targetWord[3] << '\n' << endl;

                round = true;
                break;
            }
            case 3: {
                // The rules of the game will be shown. The menu will be shown again.
                cout << "Rules:" << endl;
                cout << "You will have 8 attempts to determine the target word." << endl;
                cout << "The target word is 7 characters in length." << endl;
                cout << "Your guess words may be 3-7 characters in length" << endl;
                cout << "All guess inputs must be 7 characters in length. " << endl;
                cout << "       -Add underscores to words to reach 7 characters. (ie: 'Candy__' OR '__both_)" << endl;
                cout << "Each valid input guess will return a definition and part of speech." << '\n' << endl;

                cout << "^ = Letter is in the word and in the correct spot" << endl;
                cout << "* = Letter is in the word but in the wrong spot" << endl;
                cout << "# = Letter is not in the word in any spot" << '\n' << endl;
                continue;
            }
            case 4: {
                // The while loop will be terminated.
                cout << "Thanks for playing!" << endl;
                game = false;
                continue;
            }
            default: {
                // An invalid input will be cleared & ignored, the menu will be shown again.
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                cout << "ERROR: Invalid input. Please input a number 1-4." << endl;
                continue;
            }
        }

        while (round) {

            if (attempt > 8) { // Running out of attempts.
                cout << "You lose! You ran out of attempts. The target word was: " << targetWord[0] << endl;
                cout << "Target Part of Speech: " << targetWord[2] << endl;
                cout << "Target Definition: " << targetWord[3] << '\n' << endl;
                round = false;
                break;
            }

            // Request input.
            string input;
            cout << "Enter your guess word. OR enter 1 to print target word details. OR enter 2 to end round." << endl;
            cin >> input;

            // Target word is correctly inputted.
            if(input.compare(targetWord[0]) == 0) {
                cout << "You win! The target word was: " << targetWord[0] << endl;
                cout << "Target Part of Speech: " << targetWord[2] << endl;
                cout << "Target Definition: " << targetWord[3] << '\n' << endl;
                round = false;
                break;
            }

            // Call function to validate input.
            int response = validateInput(input, grid, attempt, targetWord);
            if(response == 1){
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                continue;
            }
            else if(response == 2){
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                round = false;
                break;
            }

            // Search input word in structures.
            string inputWord = removeUnderscores(input);
            vector<string> searchWord;
            loadingTime = 0;

            if(menu == 1) { // RB Tree search.
                auto start = chrono::steady_clock::now();
                searchWord = treeRB.search(inputWord);
                auto end = chrono::steady_clock::now();
                double loadingTimeNS = double (chrono::duration_cast <chrono::nanoseconds> (end-start).count());
                loadingTime = loadingTimeNS/(1e9);
                cout << "Time taken to search input in RB tree = " << loadingTime << " seconds" << endl;
            }
            if(menu == 2) { // Hashmap search.
                auto start = chrono::steady_clock::now();
                searchWord = hashMap.search(inputWord);
                auto end = chrono::steady_clock::now();
                double loadingTimeNS = double (chrono::duration_cast <chrono::nanoseconds> (end-start).count());
                loadingTime = loadingTimeNS/(1e9);
                cout << "Time taken to search input in Hash Map = " << loadingTime << " seconds" << endl;
            }
            if(searchWord.empty()){
                cout << "ERROR: Not in word list!" << endl;
                continue;
            }
            // Print your guess & it's details.
            cout << "\nYour guess: " << inputWord << endl;
            cout << "Part of Speech: " << searchWord[2] << endl;
            cout << "Definition: " << searchWord[3] << endl;
            cout << '\n';

            // Determine the accuracy of the input guess using symbols.
            grid[attempt].first = input;
            grid[attempt].second = symbolResults(targetWord[0], input);
            attempt++;

            // Print grid
            printGrid(grid, attempt);
        }
        // Reset the grid for the next round.
        resetGrid(grid);
    }
    return 0;
}

// Insert all data points into the tree using the insert function from
double loadRBTree(RBTree &treeRB) {
    double loadingTime = 0;
    // Open input file.
    ifstream inFile;
    inFile.open("unique_words (1).csv");

    // Skip header line.
    string line = "";
    getline(inFile, line);
    line = "";

    // Begin timer.
    auto start = chrono::steady_clock::now();

    while(getline(inFile, line)) {

        string wordID;
        string word;
        int length;
        string partOfSpeech;
        string definition;
        string tempString;

        // Read in variables from file for each word.
        stringstream inputString(line);
        getline(inputString, wordID, ',');
        getline(inputString, word, ',');

        // If length read in is a digit, add it to the RB tree. If not, length will be 0.
        getline(inputString, tempString, ',');
        bool digit = true;
        for(int i = 0; i < tempString.length(); i++){
            if(isdigit(tempString[i]) == 0) {
                digit = false;
                break;
            }
        }
        if(digit) {
            length = stoi(tempString);
        }
        else {
            length = 0;
        }
        // Read in rest of variables.
        getline(inputString, partOfSpeech, ',');
        getline(inputString, definition);

        // Call function to insert the word and its variables into the RB tree.
        treeRB.insert(word, length, partOfSpeech, definition);

        line = "";
    }
    // End timer.
    auto end = chrono::steady_clock::now();

    // Calculate time elapsed.
    double loadingTimeNS = double (chrono::duration_cast <chrono::nanoseconds> (end-start).count());
    loadingTime = loadingTimeNS/(1e9);
    inFile.close();
    return loadingTime;
}


// Insert all data points into the tree using the hashmap insert function.
double loadHashmap(HashMap &hashMP) {
    double loadingTime = 0;
    // Open input file.
    ifstream inFile;
    inFile.open("unique_words (1).csv");

    // Skip header line.
    string line = "";
    getline(inFile, line);
    line = "";

    // Begin timer.
    auto start = chrono::steady_clock::now();

    while(getline(inFile, line)) {

        string wordID;
        string word;
        int length;
        string partOfSpeech;
        string definition;
        string tempString;

        // Read in variables from file for each word.
        stringstream inputString(line);
        getline(inputString, wordID, ',');
        getline(inputString, word, ',');

        // If length read in is a digit, add it to the RB tree. If not, length will be 0.
        getline(inputString, tempString, ',');
        bool digit = true;
        for(int i = 0; i < tempString.length(); i++){
            if(isdigit(tempString[i]) == 0) {
                digit = false;
                break;
            }
        }
        if(digit) {
            length = stoi(tempString);
        }
        else {
            length = 0;
        }
        // Read in rest of variables.
        getline(inputString, partOfSpeech, ',');
        getline(inputString, definition);

        // Call function to insert the word and its variables into the hashmap.
        hashMP.insert(word, length, partOfSpeech, definition);

        line = "";
    }
    // End timer.
    auto end = chrono::steady_clock::now();

    // Calculate time elapsed.
    double loadingTimeNS = double (chrono::duration_cast <chrono::nanoseconds> (end-start).count());
    loadingTime = loadingTimeNS/(1e9);
    inFile.close();
    return loadingTime;
}


// Create grid of 7 rows.
void generateGrid(vector<pair<string,string>> &grid) {
    for(int i = 0; i < 7; i++){
        pair<string, string> row;
        row = make_pair("_______", "");
        grid.push_back(row);
    }
}

// Print current grid showing game round record of input guesses and empty rows.
void printGrid(vector<pair<string,string>> &grid, int attempt) {

    // Print guesses/symbols of the round so far.
    int num;
    for (int i = 0; i < attempt; i++) {
        // Print input guess word.
        num = i + 1;
        cout << num << ". " << (grid[i].first)[0] << " " << (grid[i].first)[1] << " " << (grid[i].first)[2] << " "
             << (grid[i].first)[3] << " " << (grid[i].first)[4] << " " << (grid[i].first)[5] << " "
             << (grid[i].first)[6] << endl;

        // Print symbols beneath.
        cout << "   " << (grid[i].second)[0] << " " << (grid[i].second)[1] << " " << (grid[i].second)[2] << " "
             << (grid[i].second)[3] << " " << (grid[i].second)[4] << " " << (grid[i].second)[5] << " "
             << (grid[i].second)[6] << endl;
        cout << '\n';
    }
    // Print empty rows.
    attempt++;
    for (int i = attempt; i <= 8; i++) {
        cout << i << ". _ _ _ _ _ _ _" << '\n' << endl;
    }
}

// Replace grid with underscores and blank values.
void resetGrid(vector<pair<string,string>> &grid) {
    for(int i = 0; i < 7; i++){
        grid[i].first = "_______";
        grid[i].second = "";
    }
}

// Remove underscore characters from input in order to search in structures.
string removeUnderscores(string word) {
    string output = "";
    // Create new string by adding all characters except underscores.
    for(int i = 0; i < word.length(); i++) {
        if(word[i] != '_'){
            output = output + word[i];
        }
    }
    return output;
}

// Validate input. Determine the following action.
int validateInput(string input, vector<pair<string,string>> &grid, int attempt, vector<string> targetWord) {

    // 1 = Ask for input again.
    // 2 = End round.
    // 3 = other

    // Reprint target word details & grid.
    if (input.compare("1") == 0) {
        printGrid(grid, attempt);

        cout << "Target Length: " << targetWord[1] << endl;
        cout << "Target Part of Speech: " << targetWord[2] << endl;
        cout << "Target Definition: " << targetWord[3] << '\n' << endl;
        return 1;
    }
    // End current round.
    if (input.compare("2") == 0) {
        cout << "Round ended." << endl;
        return 2;
    }
    // Input is invalid length.
    if (input.length() != 7) {
        cout << "ERROR: Your guess input must be 7 characters long!" << endl;
        return 1;
    }
    // Input is not solely alphabetic characters or underscores.
    bool validInput = true;
    for (int i = 0; i < input.length(); i++) {
        if ((isalpha(input[i]) == 0) && input[i] != '_') {
            validInput = false;
        }
    }
    if (!validInput) {
        cout << "ERROR: Input guess may only be characters in the alphabet or underscores!" << endl;
        return 1;
    }
    // Input is valid. Must be checked for presence in dataset.
    return 3;
}

// Returns a string of symbols representing the accuracy of a valid input guess.
string symbolResults(string targetWord, string input) {
    string symbols = "";
    for(int i = 0; i < targetWord.length(); i++) {
        if (input[i] == targetWord[i])
            symbols += '^'; // Character is in the target word in the same position.
        else if (targetWord.find(input[i]) != string::npos)
            symbols += '*'; // Character is in the target word but in a different position.
        else
            symbols += '#'; // Character is not in the target word.
    }
    return symbols;
}
