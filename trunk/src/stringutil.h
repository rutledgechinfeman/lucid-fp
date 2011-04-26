#ifndef STRINGUTIL_H
#define STRINGUTIL_H

#include <string>
#include <vector>
using namespace std;

class StringUtil
{
public:
    StringUtil();
    static void split(string toSplit, const char* on, vector<string> &result);
    static string trim(string in);
};

#endif // STRINGUTIL_H
