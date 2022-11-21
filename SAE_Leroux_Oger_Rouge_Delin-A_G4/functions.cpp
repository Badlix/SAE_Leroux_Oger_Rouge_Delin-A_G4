#include "functions.h"
#include <iostream>
#include <fstream>
#include <algorithm>

using namespace std;

// options = list of options for which you can vote
// votes = list of all votes
// scores = list of the number of vote/points that each options have

//----------    Functions used to get and parse data from files    ----------//

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

vector<string> functions::getVoters(ifstream & file) {
    vector<string> voters;
    string tmpNom;
    string tmpPrenom;
    while(true){

        getline(file,tmpNom);
        getline(file,tmpPrenom);
        if (file.eof()) break;
        voters.push_back(tmpNom + " " + tmpPrenom );
    }
    return voters;
}

vector<string> functions::separateWords(const string & line, const char & delimiter) {
    // used to separate a string into a vector of string depending of a delimiter
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

//----------   Functions use to check the validity of entries   ----------//

bool functions::isEntryValid_ranked(const vector<string> & options, const vector<unsigned> & vote) {
    // use in the case where options need to be ranked between 1 and options.size()
    // use in the borda scores and the instant run-off voting
    if (vote.size() != options.size()) return false;
    unsigned expectedValue (1); //goes from 1 to options.size()
    for (size_t i = 0; i < options.size(); ++i) {
        if(find(vote.begin(), vote.end(), expectedValue) == vote.end()) return false; // expected value was not find
        ++expectedValue;
    }
    return true;
}

bool functions::isEntryValid_Unique(const vector<string> & options, const vector<unsigned> & vote){
    // check that there is only one vote
    // use in the First-past-the-post voting
    unsigned counterUpVotes = 0; // It scoress the number of 1 in a vote.
    if (options.size() != vote.size()) return false;
    for(unsigned value : vote){
        if (value == 0) continue;
        if (value == 1) {
            if (counterUpVotes > 0) return false; // There are more than a vote
            ++counterUpVotes;
        }
        else return false;
    }
    if (counterUpVotes == 0) return false; // There are no vote
    return true;
}

bool functions::isEntryValid_Binary(const vector<string> & options, const vector <unsigned> & vote)
/* Test function that verify if the vote is correct
 * (If it contains only zeros and ones) */
// use in the approval voting
{
    if (options.size() != vote.size()) return false;
    for ( size_t i = 0 ; i < vote.size() ; ++i){
        if (vote[i] != 0 || vote[i] != 1) return false ;
    }
    return true ;
}

// A VERIF
vector<vector<unsigned>> functions::sortValidVoteEntry(vector<string> & options, vector<vector<unsigned>> & votes){
    for (unsigned i(0);i<votes.size();++i){
        bool VoF = functions::isEntryValid_Unique(options,votes[i]);
        if (VoF == false) votes.erase(votes.begin() + i);
    }
    return votes;
}

//----------   General functions used in every vote simulation   ----------//

string functions::draw(vector<unsigned> & scores, const vector<string> & options, unsigned & winnerIndex, unsigned index)
/* Recurcive function called in case of a draw.
 * Takes in parameters the result of the vote scoresing, the list of options, the index of one of the winning candidate, and an index in order to run threw the list.
 * Returns a string in the form of "winner1, winner2, ... , winnerN"
 * 'winner' is the name of a winning candidate. */
{
    if(index == scores.size()-1){                                                                        //Stop condition
        if (scores[index] == scores[winnerIndex]){
            return options[index];
        }
        else {
            return "";
        }
    }
    if (scores[index] == scores[winnerIndex]){
        return options[index]+ ", " + draw(scores, options, winnerIndex, index+1); //Recursive -> if draw
    }
    else{
        return draw(scores, options, winnerIndex, index+1);                                 //Recursive -> if not draw
    }
}

string functions::winning(vector <unsigned> & scores, const vector <string> & options)
/*Gives the winner of the vote by approval election.
 * Takes in parameter the result of the vote scoresing and the list of options.
 * Returns a string containing the name of the winning candidate.
 * In case of a draw, another function is called. */
{
    unsigned winnerIndex = 0;
    for ( size_t i = 0 ; i < options.size() ; ++i){
        if ( scores[i] > scores[winnerIndex] )
            winnerIndex = i ;
    }
    string winner;
    for ( size_t i = 0 ; i < options.size() ; ++i){
        if ( scores[i] == scores[winnerIndex] && i != winnerIndex){
            winner = "Draw between : " + functions::draw(scores, options, winnerIndex, 0);
            break;
        }
        else {
            winner = "Winner : " + options[winnerIndex];
        }
    }
    return winner ;
}

//----------   Voting functions   ----------//

vector<unsigned> functions::calcScore_FPTP(const vector<string> & options, const vector<vector<unsigned>> & votes){
    vector<unsigned> scores(options.size(),0);
    for (vector<unsigned> vote : votes){
        for (unsigned i(0) ; i < vote.size() ; ++i){
            if (vote[i] == 1) scores[i] += 1;
        }
    }
    return scores;
}

// A FINIR
vector<unsigned> functions::calcScore_IRVVoting(const vector<string> & options, const vector<vector<unsigned>> & votes) {
    vector<unsigned> scores(options.size(), 0);
    //1er tour
    scores = functions::calcScore_FPTP(options, votes);
 //   unsigned max (0);
    //unsigned sum (0);
//    for (unsigned number : scores) {
//        if (number > max) max = number;
//        sum += number;
//    }

    return scores;
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

vector<unsigned> functions::calcScore_Approval(const vector<vector<unsigned>> & votes, const vector<string> & options)
/* Simulating the vote scoresing from the ballot box (= l'urne).
 * Takes in parameters the votes from the previous function and the list of options.
 * Returns the result of the scoresing in the form of [votesForCandidate1, votesForCandidate2, ... ,votesForCandidateN].
 * 'votesForCandidate' is an integer of the number of votes they got.
 * The size of the vector equals the number of options. */
{
    vector<unsigned> scores(options.size()-1, 0);
    for ( size_t i = 0 ; i < votes.size() ; ++i ){
        for ( size_t j = 0 ; j < votes[i].size() ; ++j ){
            scores[j] += votes[i][j];
        }
    }
    return scores;
}

//----------   Functions used to print variables, often during tests.   ----------//

void functions::printVectorOfVector(const vector<vector<unsigned>> & vectorOfVector) {
    for (const vector<unsigned> &vector : vectorOfVector) {
        for (unsigned value : vector) {
            cout << value << "  ";
        }
        cout << endl;
    }
}

void functions::printVectorOfString(const vector<string> & vector) {
    for (size_t i = 0 ; i < vector.size() ; ++i) {
        cout << vector[i] << endl ;
    }
    cout << endl;
}

void functions::printVectorOfUnsigned(const vector<unsigned> & vector) {
    for (size_t i = 0 ; i <= vector.size() ; ++i) {
        cout << vector[i] << "  " ;
    }
    cout << endl;
}

//----------   Functions used to generate random vote   ----------//

vector <unsigned> functions::votingByApproval (vector<string> & options)
/* Simulating a singular vote from an individual.
 * Takes in parameters the list of options to decide from wich to vote.
 * Returns a random vote in the form of [vote1,vote2,vote3, ... ,voteN]. Each vote has either a value of 1 or 0.
 * 1 is an approval, 0 is a disapproval. The size of the vector equals the number of options. */
{
    vector <unsigned> vote (options.size());
    for ( size_t i = 0 ; i <= options.size() ; ++i){
        vote[i] = rand()%2;

    }
    return vote ;
}

vector<vector<unsigned>> functions::electionByApproval(vector<string> & voters, vector<string> & options)
/* Simulating the vote from all the individuals from 'voters'.
 * Takes in parameters the list of voters and the list of options
 * Returns a list of all the votes in the form of [voteFromVoter1, voteFromVoter2, ... , voteFromVoterN].
 * 'voteFromVoter' is the result of the previous function.
 * The size of the vector equals the number of voters. */
{
    srand (time(NULL));
    vector <vector <unsigned>> votes (voters.size());
    for ( size_t i = 0 ; i < voters.size() ; ++i){
        votes[i] = votingByApproval(options);
    }
    return votes;
}

void functions::completeSimulationApprovalVote(vector<string> & voters, vector<string> & options)
/* Simulates entirely a vote by approval.
 * Takes in parameters the list of voters and the list of options.
 * Prints the results of the elections. */
{
    vector<vector<unsigned>> votes ;
    votes = electionByApproval(voters, options);
    vector<unsigned> scores ;
    scores = calcScore_Approval(votes, options);
    cout << winning(scores, options) << endl ;
}







