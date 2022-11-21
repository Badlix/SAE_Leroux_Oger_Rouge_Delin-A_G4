#ifndef FUNCTIONS_H
#define FUNCTIONS_H
#include <iostream>
#include <vector>

using namespace std;

namespace functions
{
    // Functions used to get and parse data from files
    vector<string> getListOptions(ifstream & file);
    vector<vector<unsigned>> getListVotes(ifstream & file);
    vector<string> getVoters(ifstream & file);
    vector<string> separateWords(const string & line, const char & delimiter);

    // Functions use to check the validity of entries
    bool isEntryValid_ranked(const vector<string> & options, const vector<unsigned> & vote); // use for vote system where all the options are ranked
    bool isEntryValid_Unique(const vector<string> & options, const vector<unsigned> & vote);
    bool isEntryValid_Binary(const vector<string> & options, const vector<unsigned> & vote);
    vector<vector<unsigned>> sortValidVoteEntry(vector<string> & options, vector<vector<unsigned>> & votes);

    // General functions used in every vote simulation
    string draw(vector<unsigned> & scores, const vector<string> & options, unsigned & winnerIndex, unsigned index);
    string winning(vector<unsigned> & scores, const vector<string> & options);

    // Voting functions
    vector<unsigned> calcScore_FPTP(const vector<string> & options, const vector<vector<unsigned>> & votes);
    vector<unsigned> calcScore_IRVVoting(const vector<string> & options, const vector<vector<unsigned>> & votes);
    vector<unsigned> calcScore_Borda(const vector<string> & options, const vector<vector<unsigned>> & votes);
    vector<unsigned> calcScore_Approval(const vector<vector<unsigned>> & votes, const vector<string> & candidates);

    // Functions used to print variables, often during tests.
    void printVectorOfVector(const vector<vector<unsigned>> & vectorOfVector);
    void printVectorOfUnsigned(const vector<unsigned> & vector);
    void printVectorOfString(const vector<string> & vector);

    // Functions used to generate random vote
    vector<unsigned> votingByApproval(vector<string> & candidates);
    vector<vector<unsigned>> electionByApproval(vector<string> & voters, vector<string> & candidates);
    void completeSimulationApprovalVote(vector<string> & voters, vector<string> & candidates);
}

#endif // FUNCTIONS_H
