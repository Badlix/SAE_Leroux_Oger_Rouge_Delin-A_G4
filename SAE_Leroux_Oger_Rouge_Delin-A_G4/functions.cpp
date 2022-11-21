#include "functions.h"
#include <iostream>
#include <fstream>

using namespace std;

vector<string> functions::separateWords(const string & line, const char & delimiter) {
    size_t indexBegin (0);
    size_t indexEnd;
    vector<string> words;
    while (indexBegin < line.size()) {
        indexEnd = line.find_first_of(delimiter, indexBegin);
        words.push_back(line.substr(indexBegin, indexEnd-indexBegin));
        indexBegin += words.back().size() + 1; // 1 = delimiter size
    }
    return words;
}

vector<string> functions::getListOptions(ifstream & file) {
    vector<string> options;
    string firstLine;
    getline(file, firstLine, '\n');
    options = separateWords(firstLine, '-');
    return options;
}

vector<vector<string>> functions::getListVotes(ifstream & file) {
    string line;
    vector<vector<string>> votes;
    getline(file,line); // we don't need the first line since it represents the options
    while(true) {
        getline(file, line);
        if (file.eof()) break;
        votes.push_back(separateWords(line, '-'));
    }
    return votes;
}







