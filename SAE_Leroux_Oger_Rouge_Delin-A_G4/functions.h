#ifndef FUNCTIONS_H
#define FUNCTIONS_H
#include <iostream>
#include <vector>

using namespace std;

namespace functions
{
    vector<string> getListOptions(ifstream & file);
    vector<vector<unsigned>> getListVotes(ifstream & file);
    vector<string> separateWords(const string & line, const char & delimiter);
    void printVectorOfVector(const vector<vector<unsigned>> & vectorOfVector);
    bool isEntryValid_ranked(const vector<unsigned> & vote, const vector<string> & options); // use for vote system where all the options are ranked
}

#endif // FUNCTIONS_H
