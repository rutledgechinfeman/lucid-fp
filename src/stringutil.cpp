#include "stringutil.h"

StringUtil::StringUtil()
{
}

void StringUtil::split(string toSplit, const char* on, vector<string> &result)
{
    int index;
    while ((index = toSplit.find(on)) != -1)
    {
        result.push_back(toSplit.substr(0, index));

        toSplit = toSplit.substr(index+1, toSplit.size());
    }

    result.push_back(toSplit);
}


string StringUtil::trim(string in)
{
    while(in.find(" ") == 0)
    {
        in = in.substr(1);
    }

    while(in.find_last_of(" ") == in.size()-1 && in.size() != 0)
    {
        in = in.substr(0,in.size()-1);
    }

    return in;
}
