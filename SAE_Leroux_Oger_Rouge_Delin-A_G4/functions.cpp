#include "functions.h"
#include <iostream>
#include <fstream>
#include <algorithm>

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

vector<vector<unsigned>> functions::getListVotes(ifstream & file) {
    string line;
    vector<string> ListvaluesStr;
    vector<unsigned> listValuesUnsigned;
    vector<vector<unsigned>> votes;  
    while(true) {
        getline(file, line);
        if (file.eof()) break;
        ListvaluesStr = separateWords(line, '-');
        listValuesUnsigned = {};
        for (const string &value : ListvaluesStr) {
            listValuesUnsigned.push_back(stoul(value));
        }
        votes.push_back(listValuesUnsigned);
    }
    return votes;
}

void functions::printVectorOfVector(const vector<vector<unsigned>> & vectorOfVector) {
    for (const vector<unsigned> &vector : vectorOfVector) {
        for (unsigned value : vector) {
            cout << value << "  ";
        }
        cout << endl;
    }
}

//vector<unsigned> functions::calcScore_Borda(vector<string> & options, vector<vector<unsigned>> & votes){
//    vector<unsigned> finalScore (options.size(),0);
//    for (unsigned i = 0 ; i < options.size() ; ++i){
//        for (unsigned j = 0 ; votes.size() ; ++j){
//            finalScore[i] += (options.size() - votes[i][j]);
//        }
//    }
//    return finalScore;
//}

bool functions::isEntryValid_ranked(const vector<string> & options, const vector<unsigned> & vote) {
    // use in the case where options need to be ranked between 1 and options.size()
    if (vote.size() != options.size()) return false;
    unsigned expectedValue (1); //goes from 1 to options.size()
    for (size_t i = 0; i < options.size(); ++i) {
        if(find(vote.begin(), vote.end(), expectedValue) == vote.end()) return false; // expected value was not find
        //cout << i <<" - ok" << endl;
        ++expectedValue;
    }
    return true;
}

bool functions::isEntryValid_Unique(const vector<string> & options, const vector<unsigned> & vote){
    unsigned counterUpVotes = 0; // It counts the number of 1 in a vote.
    if (options.size() != vote.size()) return false;
    for(unsigned value : vote){
        if (value == 0) continue;
        if (value == 1) {
            if (counterUpVotes > 0) return false;
            ++counterUpVotes;
        }
        else return false;
    }
    if (counterUpVotes == 0) return false;
    return true;
}

vector<unsigned> functions::calcScore_FPTP(const vector<string> & options, const vector<vector<unsigned>> & votes){
    vector<unsigned> score(options.size(),0);
    for (vector<unsigned> vote : votes){
        for (unsigned i(0) ; i < vote.size() ; ++i){
            if (vote[i] == 1) score[i] += 1;
        }
    }
    return score;
}

vector<unsigned> functions::calcScore_IRVVoting(const vector<string> & options, const vector<vector<unsigned>> & votes) {
    vector<unsigned> scores(options.size(), 0);
    //1er tour
    scores = functions::calcScore_FPTP(options, votes);
    unsigned max (0);
    unsigned sum (0);
    for (unsigned number : scores) {
        if (number > max) max = number;
        sum += number;
    }

    return scores;
}

// A VERIF
vector<vector<unsigned>> functions::TrueVoteEntry(vector<string> & options, vector<vector<unsigned>> & votes){
    for (unsigned i(0);i<votes.size();++i){
        bool VoF = functions::isVoteEntry_Unique(options,votes[i]);
        if (VoF == false) votes.erase(votes.begin() + i);
    }
    return votes;
}

vector<unsigned> functions::calcScore_Borda(const vector<string> & options, const vector<vector<unsigned>> & votes) {
    vector<unsigned> scores(options.size(),0);
    for(unsigned i = 0 ; i < options.size() ; ++i ) {
        for(unsigned j = 0 ; j < votes.size() ; ++j) {
            scores[i] += ( options.size() - (votes[j][i]-1) );//It increase the score of the differents variable by taking the value of the number of options minus the value
        }
    }
    return scores;
}









