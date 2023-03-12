//
// SBTrie.h By: Adam Shaar
// For Implementing the game play for the "New York Times Spelling Bee"
//

#ifndef _MY_TRIE_H
#define _MY_TRIE_H

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

using namespace std;

class Trie {

protected:
  // private/protected stuff goes here!
  //  - protected allows this class to be inherited
  //    and adapted to the problem at hand (if needed)
  //   typedefs
  //   data members
  //   definition of trie node structure
  //   private helper functions
  //   etc.
  struct Trie_Node {
    Trie_Node *next[26];
    bool isWord;
    int size;

    Trie_Node() {
      size = 26;
      for (int i = 0; i < size; i++) {
        // fills the next arr with nulls for searching
        next[i] = nullptr;
      }
      isWord = false;
    }

    ~Trie_Node() {
      for (auto iter : next) {
        delete iter;
      }
    }
  };

  Trie_Node *root;
  int count;

public:
  /**
   * constructor and destructor
   */
  Trie() {
    // creates a root for calling
    root = new Trie_Node();
    count = 0;
    // your constructor code here!
  }
  ~Trie() {
    clear();
    // deletes the root memory
    delete root;
    // your destructor code here.
  }

  /*
   * function: getFromFile
   * description:  extract all of the words from the file
   *   specified by the filename given in the parameter.
   *   Words are separated by whitespace characters and
   *   must only contain letters.
   *
   * return:  indicates success/failure (file not readable...)
   */
  bool getFromFile(string filename) {
    ifstream my_file(filename);
    if (my_file.is_open()) {
      // makes sure the file is open
      string line, word;
      while (my_file >> word) {
        // storing words from file
        insert(word);
      }
      return true;
    }
    return false;
  }

  /*
   * function: insert
   * description:  inserts the word given by the parameter
   *   into the trie data structure.
   *
   *   fails if word already exists or contains non-letters
   *
   * return:  indicates success/failure
   */
  bool insert(string word) {
      Trie_Node *curr = root;
      for (char c : word) {
        if (!(isalpha(c))) {
          // makes sure the words contain letters only
          return false;
        }
        c = tolower(c);
        // converts the letters to lowercase for ascii math
        if ((curr->next[c - 'a']) == nullptr) {
          // creates a new Node if word doesn't exist yet
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

  /*
   * function: search
   * description:  determines if the word given by the parameter
   *   is stored in the trie data structure.
   *
   *   fails if word already does not exist
   *
   * return:  indicates success/failure
   */
  bool search(string word) const {
    Trie_Node *curr = root;
    for (char c : word) {
      c = tolower(c);
      if ((curr->next[c - 'a']) == nullptr) {
        // word doesn't exist yet
        return false;
      }
      curr = curr->next[c - 'a'];
    }
    return true;
  }

  /*
   * function: remove
   * description:  removes the word given by the parameter
   *   from the trie data structure.  Only deallocate nodes
   *   in subtrees that do not contain any words.
   *
   *   succeed if word exists and is properly removed
   *
   * return:  indicates success/failure
   */
  bool remove(string word) {
    if (search(word)) {
      // if the word exists
      Trie_Node *curr = root;
      for (char c : word) {
        c = tolower(c);
        curr = curr->next[c - 'a'];
      }
      curr->isWord = false;
      for (int i = 0; i < curr->size; i++) {
        if (curr->next[i]) {
          if (curr->next[i]->isWord) {
            return true;
          } else {
            delete curr->next[i];
            // deletes the memory of removed node
            curr->next[i] = nullptr;
          }
        }
      }
      count--;
      return true;
    }
    return false;
  }

  /*
   * function: clear
   * description:  remove all words from the trie and deallocate
   *   all nodes.
   *
   * return:  indicates success/failure
   */
  bool clear() {
    // rests the Trie
    delete root;
    root = new Trie_Node();
    count = 0;
    return true;
  }
  /*
   * function: wordCount
   * description:  return the number of words currently stored in the trie
   *
   * return:  an integer containing the number of words stored
   *
   * comment/note:  this should have an O(1) runtime.
   */
  int wordCount() const { return count; }

  void _thelper(Trie_Node *curr, string word, vector<string> &words) const {
    // recursive function for storing the Trie Values in a vector
    if (curr->isWord) {
      words.push_back(word);
    }
    for (int i = 0; i < 26; i++) {
      if (curr->next[i] != nullptr) {
        _thelper(curr->next[i], word + char('a' + i), words);
      }
    }
  }

  /*
   * function: words
   * description:  build a vector of all words in the dictionary and return that
   *   vector.  The words are to be in sorted ascending order.
   *
   * return:  a pointer to a vector of strings
   *
   */
  std::vector<string> *words() const {
    vector<string> *word = new vector<string>();
    // calls recursive function to store the Trie to the vector
    _thelper(root, "", *word);
    return word;
  }
};

#endif
