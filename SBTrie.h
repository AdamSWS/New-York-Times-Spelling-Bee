//
// SBTrie.h By: Adam Shaar
// For Implementing the game play for the "New York Times Spelling Bee"
//

#ifndef _MY_SB_TRIE_H
#define _MY_SB_TRIE_H

#include "Trie.h"
#include <iostream>
#include <string>
#include <vector>

class SBTrie : public Trie {

  // the main reason for inheriting the Trie class is that the words() method in
  // Tries is quite inefficient for the use of the Spelling Bee game.
  //
  // This allows us to customize a better method to get the desired words by
  // "adding an addtional method" on the the Trie class at the cost of using
  // inheritance.

protected:
  // private/protected stuff goes here!
  //  - protected allows this class to be inherited
  //    and adapted to the problem at hand (if needed)
  //   typedefs
  //   data members
  //   definition of trie node structure
  //   private helper functions
  //   etc.
  Trie_Node *root;
  Trie_Node *found;
  char central_letter;
  string allowed_letters;
  int current_score;
  bool pangram_found;
  bool bingo_found;

public:
  /**
   * constructor and destructor
   */
  SBTrie() {
    found = new Trie_Node();
    current_score = 0;
    pangram_found = false;
    bingo_found = false;
    // your constructor code here!
  }
  ~SBTrie() {
    delete [] found;
    // your destructor code here.
  }

  int calculateScore(string letters) {
    // default score if the word is a length of 4
    int score = 1;
    if (letters.length() != 4) {
      // sets score equal to the length
      score = letters.length();
      if (isWordPangram(letters)) {
        // adds a bonus of 7 if the user finds a pangram
        score = score + 7;
      }
    }
    return score;
  }

  bool insertFound(string word) {
    // inserts a new Trie_Node in the found Trie
    Trie_Node *curr = found;
    for (char c : word) {
      if (!(isalpha(c))) {
        // making sure the characters are valid
        return false;
      }
      c = tolower(c);
      // setting the casing to lower for ascii math
      if ((curr->next[c - 'a']) == nullptr) {
        // creates new node if null
        curr->next[c - 'a'] = new Trie_Node();
      }
      curr = curr->next[c - 'a'];
    }
    if (!curr->isWord) {
      curr->isWord = true;
      count++;
      return true;
    }
    return false;
  }

  bool isWordPangram(string letters) {
    if (letters.length() < 7) {
      // default case: Must be of length 7
      return false;
    }
    bool chars[7] = {false};
    string complete = allowed_letters + central_letter;
    for (char &c : complete) {
      for (char c2 : letters) {
        // O(n^2) search, but the data is small 
        if (c == c2) {
          c = 'X';
          break;
        }
      }
    }
    if (complete == "XXXXXXX") {
      // pangram has been detected
      pangram_found = true;
      return true;
    }
    return false;
  }

  bool isBingoScored() {
    vector<string> *foundWords = new vector<string>();
    _thelper(found, "", *foundWords);
    if (foundWords->size() < 7) {
      // default case: must have found 7 words to even be considered
      return false;
    }
    bool chars[7] = {false};
    string complete = allowed_letters + central_letter;
    for (char &c : complete) {
      for (string str : *foundWords) {
        // O(n^2) search, pretty bulky data
        char c2 = str[0];
        // takes the first character of the found strings
        if (c == c2) {
          c = 'X';
          break;
        }
      }
    }
    if (complete == "XXXXXXX") {
      // if a bingo is found
      bingo_found = true;
      return true;
    }
    return false;
  }

  bool searchFound(string word) const {
    Trie_Node *curr = found;
    for (char c : word) {
      c = tolower(c);
      if ((curr->next[c - 'a']) == nullptr) {
        return false;
      }
      curr = curr->next[c - 'a'];
    }
    // traversed to the pos of word and haven't hit nullptr
    return true;
  }

  bool tooShort(string letters) const { return letters.length() < 4; }
  // makes sure the word is at least 4 letters long

  bool missingCentralLetter(string letters, char centralLetter) const {
    // makes sure the word contains the central letter
    return letters.find(centralLetter) == string::npos;
  }

  bool containsInvalidLetter(string letters, string allowedLetters,
                             char centralLetter) const {
    // makes sure that the user didn't input invalid letter data
    for (char c : letters) {
      if (allowedLetters.find(c) == string::npos && c != centralLetter) {
        return true;
      }
    }
    return false;
  }

  // getters
  char getCentralLetter() { return central_letter; }
  string getAllowedLetters() { return allowed_letters; }
  int getScore() { return current_score; }
  Trie_Node *getFound() { return found; }
  bool isPangramFound() { return pangram_found; }
  // setters
  void setCentralLetter(char c) { central_letter = c; }
  void setAllowedLetters(string s) { allowed_letters = s; }
  void setScore(int s) { current_score = s; }

  /*
   * function: sbWords
   * description:  build a vector of all words in the dictionary that
   *   are valid words for the spelling bee problem and return that
   *   vector.  The words are to be in sorted ascending order.
   *   The words included must have a length of 4 or greater,
   *                      must contain the central letter, and
   *                      may contain the allowed letters
   *
   * parameter: char centralLetter - the letter that MUST be contained in the
   * words string letters - the other letters that are allowed to be in words
   *
   * return:  a pointer to a vector of strings
   *
   */
  std::vector<string> *sbWords(char centralLetter, string letters) const {
    vector<string> *valid_words = new vector<string>();
    for (string a : *words()) {
      if (!tooShort(a) && !missingCentralLetter(a, centralLetter) &&
          !containsInvalidLetter(a, letters, centralLetter)) {
        // making sure the game rules are upheld
        valid_words->push_back(a);
      }
    }
    return valid_words;
  }
};

#endif
