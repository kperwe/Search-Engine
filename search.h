// Name: Kaif Perwez
// Project Name: Search Engine

#pragma once

// Include all the required directives
#include <iostream>
#include <set>
#include <map>
#include <cctype>
#include <sstream>
#include <string>
#include <algorithm>
#include<bits/stdc++.h>

using namespace std;

// Used for evaluating set union
set<string> setUnion(set<string> x, set<string> y) {
    set<string>::iterator it;
    for (it = y.begin(); it != y.end(); ++it) 
        x.insert(*it);
    return x;
}

// Used for evaluating set intersection
set<string> setIntersect(set<string> x, set<string> y) {
    set<string> z;
    set<string>::iterator it;
    for (it = x.begin(); it != x.end(); ++it)
        if (y.find(*it) != y.end())
            z.insert(*it);
    return z;
}

// Used for evaluating set difference
set<string> setDiffrence(set<string> x, set<string> y) {
    set<string> z;
    set<string>::iterator it;
    for (it = x.begin(); it != x.end(); ++it)
        if (y.find(*it) == y.end())
            z.insert(*it);
    return z;
}

/* cleanToken takes in a whitespace-separated string of characters that appears in the body text and returns a "cleaned" version of that token, ready to be stored in the index.*/
string cleanToken(string s) {
    // This for loop checks if there exists any alphabet in the string
    int found = 0;
    for (int i = 0, len = s.size(); i < len; i++) {
        if (isalpha(s[i])) {
            found = 1;
            break;
        }
    }
    // This for loop will remove any punctuation 
    // from the beginning until the first letter is encountered or 
    // return empty string if no alphabet is found
    for (int i = 0, len = s.size(); i < len; i++) {
        if (found != 1) {
            s = "";
        } else if (ispunct(s[i])) {
            s.erase(i--, 1);
            len = s.size();
        } else {
            break;
        }
    }
    /*  This for loop will remove any punctuation from
        the end until the first letter is encountered or
        return empty string if no alphabet is found*/
    for (int i = s.size() - 1; i > 0; i--) {
        if (found != 1) {
            s = "";
        } else if (ispunct(s[i])) {
            s.erase(i);
        } else {
            break;
        }
    }
    // transform string to lowercase
    transform(s.begin(), s.end(), s.begin(), ::tolower);
    return s;
}

/* The helper function gatherTokens extracts the set of unique tokens from the body text. The function returns a set of the unique cleaned tokens that appear in that body text.*/
set<string> gatherTokens(string text) {
    set<string> tokens;
    string word = "";
    for (auto x : text) {
        if (x == ' ') {
            word = cleanToken(word);
            if (word != "") {
                tokens.insert(word);
                word = "";
            }
        } else {
            word = word + x;
        }
    }
    word = cleanToken(word);
    // check if word is empty string and then insert it into the set
    if (word != "") {
        tokens.insert(word);
    }
    return tokens;
}

/* The function buildIndex reads the content from the database file and processes it into the form of an inverted index.*/
int buildIndex(string filename, map<string, set<string>>& index) {
    ifstream inFS;
    string str;
    string url;
    set <string> tokens, urlSets, urls;
    set<string>::iterator it;
    // Take file input and then build the inverted index
    inFS.open(filename);
    if (!inFS.is_open()) {
        return 0;
    }
    int sum = 0;
    while (getline(inFS, str)) {
        url = str;
        urls.insert(str);
        getline(inFS, str);
        tokens = gatherTokens(str);
        for (it = tokens.begin(); it != tokens.end(); it++) {
            urlSets = index[*it];
            urlSets.insert(url);
            index[*it] = urlSets;
        }
        sum++;
    }
    return sum;
}

// This function finds the matches for the given query.
set<string> findQueryMatches(map<string, set<string>>& index, string sentence) {
    set<string> result;
    string word = "";
    for (auto x : sentence) {
        if (x == ' ') {
            char firstLetter = word[0];
            word = cleanToken(word);
            if (firstLetter == '+') {
                result = setIntersect(result, index[word]);
            } else if (firstLetter == '-') {
                result = setDiffrence(result, index[word]);
            } else {
                result = setUnion(result, index[word]);
            }
            word = "";
        } else {
            word += x;
        }
    }
    if (word != "") {
        char firstLetter = word[0];
        word = cleanToken(word);
        if (firstLetter == '+') {
            result = setIntersect(result, index[word]);
        } else if (firstLetter == '-') {
            result = setDiffrence(result, index[word]);
        } else {
            result = setUnion(result, index[word]);
        }
        word = "";
    }
    return result;
}

/*
 This function is to be used in the main function. 
   It do the following things:
   1. It first constructs an inverted index from the contents of the database file.
   2. It prints how many web pages were processed to build the index and how many distinct words were found across all pages.
   3. It then enters a loop that prompts the user to enter a query
   4. For each query entered by the user, it find the matching pages and prints the URLs.
   5. The user presses enter (empty string) to indicate they are done and the program finishes executing.
*/
void searchEngine(string filename) {
    map<string, set<string>> index;
    int numberOfPages = buildIndex(filename, index);
    int totalTerms = index.size();
    cout << "Stand by while building index..." << endl;
    cout << "Indexed " << numberOfPages << " pages containing ";
    cout << totalTerms << " unique terms" << endl << endl;
    string word;
    while (true) {
        cout << "Enter query sentence (press enter to quit): ";
        getline(cin, word);
        if (word == "") {
            cout << "Thank you for searching!" << endl;
            return;
        }
        set<string> result = findQueryMatches(index, word);
        cout << "Found " << result.size() << " matching pages" << endl;
        set<string>::iterator it;
        for (it = result.begin(); it != result.end(); it++) {
            cout << *it << endl;
        }
        cout << endl;
    }
}



       // ***********   End of search.h file *****************//





