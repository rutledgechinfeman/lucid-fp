#include "stringutil.h"

StringUtil::StringUtil()
{
}

void StringUtil::split(string toSplit, char* on, vector<string> &result)
{
    int index;
    while ((index = toSplit.find(on)) != -1)
    {
        result.push_back(toSplit.substr(0, index));

        toSplit = toSplit.substr(index+1, toSplit.size());
    }

    result.push_back(toSplit);
}
