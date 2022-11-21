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
    bool isEntryValid_ranked(const vector<string> & options, const vector<unsigned> & vote); // use for vote system where all the options are ranked
    bool isEntryValid_Unique(const vector<string> & options, const vector<unsigned> & vote);
    vector<unsigned> calcScore_FPTP(const vector<string> & options, const vector<vector<unsigned>> & votes);
    vector<unsigned> calcScore_IRVVoting(const vector<string> & options, const vector<vector<unsigned>> & votes);
    vector<unsigned> calcScore_Borda(const vector<string> & options, const vector<vector<unsigned>> & votes);
}

#endif // FUNCTIONS_H
