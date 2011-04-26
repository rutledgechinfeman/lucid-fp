#ifndef STRINGUTIL_H
#define STRINGUTIL_H

#include <string>
#include <vector>
using namespace std;

class StringUtil
{
public:
    StringUtil();
    static void split(string toSplit, char* on, vector<string> &result);

};

#endif // STRINGUTIL_H
