//
// spellb.cpp Implementation By: Adam Shaar
// Interface setup for the spelling bee game
//

#include "SBTrie.h"
#include "Trie.h"

#include <cctype>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

using std::cin;
using std::cout;
using std::string;

void getNewDictionary(SBTrie *sbt, string filename) {
  sbt->clear();
  // clear out existing dictionary
  sbt->getFromFile(filename);
  // store to dictionary from file
}

void updateDictionary(SBTrie *sbt, string filename) {
  sbt->getFromFile(filename);
  // store to existing dictionary from file
}

void setupLetters(SBTrie *sbt, string letters) {
  string temp = "";
  for (int i = 0; i < letters.length(); i++) {
    char c = letters[i];
    if (!isalpha(c)) {
      //  user didn't input a letter in alphabet
      continue;
    }
    for (int j = i + 1; j < letters.length(); j++) {
      char c2 = letters[j];
      if (c == c2) {
        //  duplicate letter found
        cout << "Invalid Letter Set" << endl;
        return;
      }
    }
    temp.push_back(tolower(c));
  }
  if (temp.length() != 7) {
    //  the final length of the set doesn't equal 7
    cout << "Invalid Letter Set" << endl;
    return;
  }
  sbt->setCentralLetter(temp[0]);
  sbt->setAllowedLetters(temp.substr(1));
}

void showLetters(SBTrie *sbt) {
  cout << "Central Letter: " << sbt->getCentralLetter() << endl;
  cout << "6 Other Letters: ";
  string temp = sbt->getAllowedLetters();
  for (int i = 0; i < temp.length(); i++) {
    // prints each letter
    cout << temp[i];
    if (i != temp.length() - 1) {
      cout << ",";
    }
  }
  cout << endl;
}

void attemptWord(SBTrie *sbt, string letters) {
  // make sure that the user input follows the rules
  if (sbt->tooShort(letters)) {
    cout << "word is too short" << endl;
    return;
  }
  if (sbt->containsInvalidLetter(letters, sbt->getAllowedLetters(), sbt->getCentralLetter()) || sbt->missingCentralLetter(letters, sbt->getCentralLetter())) {
    if (sbt->missingCentralLetter(letters, sbt->getCentralLetter())) {
      cout << "word is missing central letter" << endl;
      return;
    }
    if (sbt->containsInvalidLetter(letters, sbt->getAllowedLetters(), sbt->getCentralLetter())) {
      cout << "word contains invalid letter" << endl;
      return;
    }
  }
  
  if (!sbt->search(letters)) {
    cout << "word is not in the dictionary" << endl;
    return;
  }
  if (sbt->searchFound(letters)) {
    cout << "word has already been found" << endl;
    return;
  }
  // word is legitamite
  sbt->insertFound(letters);
  string msg1, msg2;
  if (sbt->calculateScore(letters) == 1) {
    msg1 = "1 point";
  } else {
    msg1 = to_string(sbt->calculateScore(letters)) + " points";
  }
  sbt->setScore(sbt->getScore() + sbt->calculateScore(letters));
  if (sbt->getScore() == 1) {
    msg2 = "1 point";
  } else {
    msg2 = to_string(sbt->getScore()) + " points";
  }
  cout << "found " + letters + " " + msg1 + ", total " + msg2;
  // add on extra concatinations to show bonuses
  if (sbt->isPangramFound()) {
    cout << ", Pangram found";
  }

  if (sbt->isBingoScored()) {
    cout << ", Bingo scored";
  }
  cout << endl;
  sbt->insertFound(letters);
}

void showFoundWords(SBTrie *sbt) {
  vector<string> *foundWords = new vector<string>();
  sbt->_thelper(sbt->getFound(), "", *foundWords);
  // calls recursive function to fill reference vector
  for (string a: *foundWords) {
    // alligns output
    cout << std::setw(17) << a << " " << std::right << std::setw(2) << a.length() << std::left << endl;
  }
  // display score
  string msg1, msg2;
  if (foundWords->size() == 1) {
    msg1 = "1 word";
  } else {
    msg1 = to_string(foundWords->size()) + " words";
  }
  if (sbt->getScore() == 1) {
    msg2 = "1 point";
  } else {
    msg2 = to_string(sbt->getScore()) + " points";
  }
  cout << msg1 << " found, total " + msg2;
  if (sbt->isPangramFound()) {
    cout << ", Pangram found";
  }
  if (sbt->isBingoScored()) {
    cout << ", Bingo scored";
  }
  cout << endl;
  delete foundWords;
}

void showAllWords(SBTrie *sbt) {
  // shows all valid possible words and their length
  vector<string> *words = sbt->sbWords(sbt->getCentralLetter(), sbt->getAllowedLetters());
  for (string a : *words) {
    cout << std::setw(17) << a << " " << std::right << std::setw(2) << a.length();
    if (sbt->isWordPangram(a)) {
      // prints pangram if word is a pangram
      cout << std::setw(2) << " " << "Pangram";
    }
    cout << std::left << endl;
  }
  delete words;
  // enter needed code here for command 7
}

void displayCommands() {
  cout << "\nCommands are given by digits 1 through 9\n\n";
  cout << "  1 <filename> - read in a new dictionary from a file\n";
  cout << "  2 <filename> - update the existing dictionary with words from a "
          "file\n";
  cout << "  3 <7letters> - enter a new central letter and 6 other letters\n";
  cout << "  4            - display current central letter and other letters\n";
  cout << "  5 <word>     - enter a potential word\n";
  cout << "  6            - display found words and other stats\n";
  cout << "  7            - list all possible Spelling Bee words from the "
          "dictionary\n";
  cout << "  8            - display this list of commands\n";
  cout << "  9            - quit the program\n\n";
}

int main(int argc, char **argv) {
  SBTrie *sbt = new SBTrie;

  cout << "Welcome to Spelling Bee Game\n";

  displayCommands();

  bool done = false;
  string line;
  string input;

  do {
    cout << "cmd> ";

    // read a complete line
    std::getline(std::cin, line);

    // now create a "stringstream" on the line just read
    std::stringstream ss(line);

    // clear input from any previous value
    input = "";

    // get command character
    char command;
    ss >> command;
    cout << "Debug command:" << command << "***\n";

    if (command == '1') {
      ss >> input;
      cout << std::setw(17) << input << " " << std::right << std::setw(2) <<
      input.length() << std::left << endl; cout << "Debug 1:" << input <<
      "***\n";
      getNewDictionary(sbt, input);
    }

    if (command == '2') {
      ss >> input;
      cout << "Debug 2:" << input << "***\n";
      updateDictionary(sbt, input);
    }

    if (command == '3') {
      ss >> input;
      cout << "Debug 3:" << input << "***\n";
      setupLetters(sbt, input);
    }

    if (command == '4') {
      showLetters(sbt);
    }

    if (command == '5') {
      ss >> input;
      cout << "Debug 5:" << input << "***\n";
      attemptWord(sbt, input);
    }

    if (command == '6') {
      showFoundWords(sbt);
    }

    if (command == '7') {
      showAllWords(sbt);
    }

    if (command == '8' || command == '?') {
      displayCommands();
    }

    if (command == '9' || command == 'q') {
      done = true;
    }

  } while (!done && !cin.eof());
  delete sbt;
  return 0;
}
