#ifndef FUNCTIONS_H
#define FUNCTIONS_H
#include <iostream>
#include <vector>

using namespace std;

namespace functions
{
    vector<string> getListOptions(ifstream & file);
    vector<vector<string>> getListVotes(ifstream & file);
    vector<string> separateWords(const string & line, const char & delimiter);
}

#endif // FUNCTIONS_H
