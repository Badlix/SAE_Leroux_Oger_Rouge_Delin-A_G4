#include <iostream>
#include <fstream>
#include <algorithm>
#include "functions.h"

using namespace std;
using namespace functions;

/* list of the voting systems availible =
 * - First-Past-The-Post Voting (FTPT)
 * - Approval Voting (Approval)
 * - Instant Run-Off Voting (IROV)
 * - Borda Count (Borda)
*/

int main()
{
    // functions get need to be called in order
    vector<string> listSysVote = getListVoteSys();
    vector<string> options = getListOptions();
    vector<vector<unsigned>> votes = getListVotes();
    isGlobalEntryValid(listSysVote, options, votes);
    vector<unsigned> scores;

    for (const string &voteSys : listSysVote) {
        if (voteSys == "FPTP") {
            scores = calcScore_FPTP(votes);
        } else if (voteSys == "Approval") {
            scores = calcScore_Approval(votes);
        } else if (voteSys == "Borda") {
            scores = calcScore_Borda(votes);
        }
    }
    string result = winning(scores, options);
    cout << result << endl;
    return 0;
}
