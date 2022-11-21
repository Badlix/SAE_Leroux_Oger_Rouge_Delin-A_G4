#include <iostream>
#include <fstream>
#include "functions.h"

using namespace std;
using namespace functions;

int main()
{
    ifstream file("../Tests/OneInput");
    vector<string> options = getListOptions(file); // getListOptions need to be called before getListVotes
    vector<vector<unsigned>> votes = getListVotes(file);
    //vector<unsigned> finalScore = calcScore_Borda(votes,options);
    return 0;
}
