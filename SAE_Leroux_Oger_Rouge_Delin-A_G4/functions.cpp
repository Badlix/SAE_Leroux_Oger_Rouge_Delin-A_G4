#include "functions.h"
#include <iostream>
#include <fstream>
#include <algorithm>

using namespace std;

// candidats = list of candidats for which you can vote
// votes = list of all votes
// scores = list of the number of vote/points that each candidats have
// a vote is a vector of unsigned, it has the same size as candidats

//----------    Functions used to get and parse data from files    ----------//

const vector<string> availibleSysVote = {"FPTP", "Approval", "IROV", "Borda"};
const string separate = "**********"; // separation between sets of entries

vector<string> functions::getListVoteSys() {
// return the list of the voting systems used for this set of entries
    vector<string> listVoteSys;
    string line;
    getline(cin, line, '\n');
    listVoteSys = separateWords(line, '-');
    return listVoteSys;
}

vector<string> functions::getListcandidats() {
// return the list of the candidats
    vector<string> candidats;
    string line;
    getline(cin, line, '\n');
    candidats = separateWords(line, '-');
    return candidats;
}

vector<vector<unsigned>> functions::getListVotes() {
// return the list of all votes
    string line;
    vector<string> ListvaluesStr;
    vector<unsigned> listValuesUnsigned;
    vector<vector<unsigned>> votes;
    while(true) {
        getline(cin, line);
        if (cin.eof()) break;
        if (line == separate) break;
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
// return a list with the name and surname of voters
    vector<string> voters;
    string tmpName;
    string tmpSurname;
    while(true){
        getline(file,tmpName);
        getline(file,tmpSurname);
        if (file.eof()) break;
        voters.push_back(tmpName + " " + tmpSurname);
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

bool functions::isGlobalEntryValid(const vector<string> listVoteSys, const vector<string> & candidats, const vector<vector<unsigned>> votes) {
// return error if one of the list is empty or can't be used   
    for (const string &voteSys : listVoteSys) {
        if(find(availibleSysVote.begin(), availibleSysVote.end(), voteSys) == availibleSysVote.end()) { // voteSys not find in Availible
            cout << "Uncorrect Voting Systems" << endl;
            return false;
        }
    }
    if (candidats.size() == 0) {
        cout << "None voting candidats (candidats) were found" << endl;
        return false;
    }
    if (votes.size() == 0) {
        cout << "None votes were found" << endl;
        return false;
    }
    return true;
}

// NOT USED
bool functions::isEntryValid_ranked(const vector<string> & candidats, const vector<unsigned> & vote) {
    // use in the case where candidats need to be ranked between 1 and candidats.size()
    // use in the borda scores and the instant run-off voting
    if (vote.size() != candidats.size()) return false;
    unsigned expectedValue (1); //goes from 1 to candidats.size()
    for (size_t i = 0; i < candidats.size(); ++i) {
        if(find(vote.begin(), vote.end(), expectedValue) == vote.end()) return false; // expected value was not find
        ++expectedValue;
    }
    return true;
}

bool functions::isEntryValid_unique(const vector<string> & candidats, const vector<unsigned> & vote){
    // check that there is only one vote
    // use in the First-past-the-post voting
    unsigned counterUpVotes = 0; // It scoress the number of 1 in a vote.
    if (candidats.size() != vote.size()) return false;
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

bool functions::isEntryValid_binary(const vector<string> & candidats, const vector <unsigned> & vote)
/* Test function that verify if the vote is correct
 * (If it contains only zeros and ones) */
// use in the approval voting
{
    if (candidats.size() != vote.size()) return false;
    for ( size_t i = 0 ; i < vote.size() ; ++i){
        if (vote[i] != 0 || vote[i] != 1) return false ;
    }
    return true ;
}

// const vector<string> listSysVotes = {"First-Past-The-Post", "Approval", "Instant Run-Off", "Borda Count"};
vector<vector<unsigned>> functions::sortValidVoteEntry(vector<string> & candidats, vector<vector<unsigned>> & votes, const string & sysVote){
    unsigned cmp (0) ;
    while (cmp < votes.size()) {
        if (sysVote == "First-Past-The-Post") {
            if (!isEntryValid_unique(candidats, votes[cmp])) {
                votes.erase(votes.begin() + cmp);
            } else {
                ++cmp;
            }
        } else if (sysVote == "Approval") {
            if (!isEntryValid_binary(candidats, votes[cmp])) {
                votes.erase(votes.begin() + cmp);
            } else {
                ++cmp;
            }
        } else if (sysVote == "Instant Run-Off" || sysVote == "Borda Count") {
            if (!isEntryValid_ranked(candidats, votes[cmp])) {
                votes.erase(votes.begin() + cmp);
            } else {
                ++cmp;
            }
        }
    }
    return votes;
}

//----------   General functions used in every vote simulation   ----------//

string functions::draw(vector<unsigned> & scores, const vector<string> & candidats, unsigned & winnerIndex, unsigned index)
/* Recurcive function called in case of a draw.
 * Takes in parameters the result of the vote scoresing, the list of candidats, the index of one of the winning candidate, and an index in order to run threw the list.
 * Returns a string in the form of "winner1, winner2, ... , winnerN"
 * 'winner' is the name of a winning candidate. */
{
    if(index == scores.size()-1){                  //Stop condition
        if (scores[index] == scores[winnerIndex]){
            return ", " + candidats[index];
        }
        else {
            return "";
        }
    }
    if (scores[index] == scores[winnerIndex]){
        return ", " + candidats[index] + draw(scores, candidats, winnerIndex, index+1); //Recursive -> if draw
    }
    else{
        return draw(scores, candidats, winnerIndex, index+1);  //Recursive -> if not draw
    }
}

string functions::winning(vector <unsigned> & scores, const vector <string> & candidats)
/* Returns the winner of the election.
 * Takes in parameter the result of the vote scores and the list of candidats.
 * Returns a string containing the name of the winning candidate.
 * In case of a draw, another function is called. */
{
    unsigned winnerIndex = 0;
    for ( size_t i = 0 ; i < candidats.size() ; ++i){
        if ( scores[i] > scores[winnerIndex] )
            winnerIndex = i ;
    }
    string winner;
    for ( size_t i = 0 ; i < candidats.size() ; ++i){
        if ( scores[i] == scores[winnerIndex] && i != winnerIndex){
            winner = "Draw between : " + candidats[winnerIndex] + functions::draw(scores, candidats, winnerIndex, winnerIndex+1);
            break;
        }
        else {
            winner = "Winner : " + candidats[winnerIndex];
        }
    }
    return winner ;
}

//----------   Voting functions   ----------//

vector<unsigned> functions::calcScore_FPTP(const vector<vector<unsigned>> & votes){
    vector<unsigned> scores(votes[0].size(),0);
    for (vector<unsigned> vote : votes){
        for (unsigned i(0) ; i < vote.size() ; ++i){
            if (vote[i] == 1) scores[i] += 1;
        }
    }
    return scores;
}

void functions::findEliminatedcandidats(const vector<unsigned> & scores, vector<unsigned> & indOfEliminatedcandidats) {
    unsigned min;
    unsigned iStart (0);
    while(myFind(indOfEliminatedcandidats, iStart) != -1) {
        ++iStart;
    }
    min = scores[iStart];
    for (size_t i (iStart) ; i < scores.size() ; ++i) {
        if (min > scores[i] && myFind(indOfEliminatedcandidats, i) == -1) {
            min = scores[i];
        }
    }
    for (size_t i = iStart; i < scores.size(); ++i) {
        if (scores[i] == min && myFind(indOfEliminatedcandidats, i)) {
            indOfEliminatedcandidats.push_back(i);
        }
    }
}
// A FINIR
vector<unsigned> functions::calcScore_IRVVoting(const vector<vector<unsigned>> & votes) {
    vector<unsigned> indOfeliminatedcandidats = {};
    vector<unsigned> scores (votes[0].size(), 0);
    unsigned rankNumber; // index of the
    while(true) {
        fill(scores.begin(),scores.end(), 0);
        for (size_t i = 0; i < votes.size(); ++i) { // check tt les votes
            rankNumber = 1;
            while (myFind(indOfeliminatedcandidats,myFind(votes[i], rankNumber)) != -1) {
                ++rankNumber;
                if (rankNumber > votes[0].size()) {
                    cout << "erreur";
                    return {1};
                }
            }
            scores[myFind(votes[i], rankNumber)] += 1;
        }
        unsigned max (0);
        unsigned sum (0);
        for (unsigned number : scores) {
            if (number > max) max = number;
            sum += number;
        }
        if (max*2 < sum) { // None candidats have the majority (+50%)
            findEliminatedcandidats(scores, indOfeliminatedcandidats);
        }
        else {
            break;
        }
    }
    return scores;
}

vector<unsigned> functions::calcScore_Borda(const vector<vector<unsigned>> & votes) {
    vector<unsigned> scores(votes[0].size(), 0);
    for(unsigned i = 0 ; i < votes[0].size() ; ++i ) {
        for(unsigned j = 0 ; j < votes.size() ; ++j) {
            scores[i] += (votes[0].size() - (votes[j][i]-1));//It increase the score of the differents variable by taking the value of the number of candidats minus the value
        }
    }
    return scores;
}

vector<unsigned> functions::calcScore_Approval(const vector<vector<unsigned>> & votes)
/* Simulating the vote scoresing from the ballot box (= l'urne).
 * Takes in parameters the votes from the previous function and the list of candidats.
 * Returns the result of the scoresing in the form of [votesForCandidate1, votesForCandidate2, ... ,votesForCandidateN].
 * 'votesForCandidate' is an integer of the number of votes they got.
 * The size of the vector equals the number of candidats. */
{
    vector<unsigned> scores(votes[0].size(), 0);
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
    for (size_t i = 0 ; i < vector.size() ; ++i) {
        cout << vector[i] << "  " ;
    }
    cout << endl;
}

//----------   Other   ----------//

vector <unsigned> functions::votingByApproval (vector<string> & candidats)
/* Simulating a singular vote from an individual.
 * Takes in parameters the list of candidats to decide from wich to vote.
 * Returns a random vote in the form of [vote1,vote2,vote3, ... ,voteN]. Each vote has either a value of 1 or 0.
 * 1 is an approval, 0 is a disapproval. The size of the vector equals the number of candidats. */
{
    vector <unsigned> vote (candidats.size());
    for ( size_t i = 0 ; i <= candidats.size() ; ++i){
        vote[i] = rand()%2;

    }
    return vote ;
}

vector<vector<unsigned>> functions::electionByApproval(vector<string> & voters, vector<string> & candidats)
/* Simulating the vote from all the individuals from 'voters'.
 * Takes in parameters the list of voters and the list of candidats
 * Returns a list of all the votes in the form of [voteFromVoter1, voteFromVoter2, ... , voteFromVoterN].
 * 'voteFromVoter' is the result of the previous function.
 * The size of the vector equals the number of voters. */
{
    srand (time(NULL));
    vector <vector <unsigned>> votes (voters.size());
    for ( size_t i = 0 ; i < voters.size() ; ++i){
        votes[i] = votingByApproval(candidats);
    }
    return votes;
}

int functions::myFind(vector<unsigned> vote, unsigned nbToFind) {
    // return index of the element in the vector
    for (size_t i = 0; i < vote.size(); ++i) {
        if (vote[i] == nbToFind) {
            return i;
        }
    }
    return -1;
}

//void functions::completeSimulationApprovalVote(vector<string> & voters, vector<string> & candidats)
///* Simulates entirely a vote by approval.
// * Takes in parameters the list of voters and the list of candidats.
// * Prints the results of the elections. */
//{
//    vector<vector<unsigned>> votes ;
//    votes = electionByApproval(voters, candidats);
//    vector<unsigned> scores ;
//    scores = calcScore_Approval(votes, candidats);
//    cout << winning(scores, candidats) << endl ;
//}







