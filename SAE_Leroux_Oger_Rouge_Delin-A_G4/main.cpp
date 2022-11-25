#include <iostream>
#include <fstream>
#include <algorithm>
#include "functions.h"

using namespace std;
using namespace functions;

/* list of the voting systems availible :
 * - First-Past-The-Post Voting (FTPT)
 * - Approval Voting (Approval)
 * - Instant Run-Off Voting (IROV)
 * - Borda Count (Borda)
*/

int main()
{
    // functions 'get' need to be called in order
    bool endOfFile = false;
    while(!endOfFile) {
        vector<string> listSysVote = getListVoteSys();
        vector<string> candidats = getListcandidats();
        vector<vector<unsigned>> votes = getListVotes();
        isGlobalEntryValid(listSysVote, candidats, votes); // check if inputs are usable
        if (cin.eof()) {
            endOfFile = true;
        }
        vector<unsigned> scores;
        string result;
        for (const string &voteSys : listSysVote) {
        // selects which fonction is used to calculte scores based on the voting system
            if (voteSys == "FPTP") {
                scores = calcScore_FPTP(votes);
            } else if (voteSys == "Approval") {
                scores = calcScore_Approval(votes);
            } else if (voteSys == "Borda") {
                scores = calcScore_Borda(votes);
            } else if (voteSys == "IROV") {
                scores = calcScore_IRVVoting(votes);
            }
            result = winning(scores, candidats);
            cout << voteSys << " - " << result << endl;
        }
        if(!endOfFile) {
            cout << "----------" << endl; // delimitations between set of entries
        }
    }
    return 0;
}
