#include <iostream>
#include <fstream>
#include "functions.h"

using namespace std;
using namespace functions;

int main()
{
    ifstream file("../Tests/OneInput");
    vector<vector<unsigned>> listVotes = getListVotes(file);
    printVectoreOfVector(listVotes);
    return 0;
}
