#include "stringutil.h"

StringUtil::StringUtil()
{
}

void StringUtil::split(string toSplit, const char* on, vector<string> &result)
{
    int index;
    while ((index = toSplit.find(on)) != -1)
    {
        // Catch the first space
        string candidate = toSplit.substr(0, index);

        // Don't push empty strings in (ie, when you have two spaces in a row)
        if(candidate.size() != 0) result.push_back(candidate);

        // Cut off the parts of the string already considered
        toSplit = trim(toSplit.substr(index+1, toSplit.size()));
    }

    // Push back the last word
    if(toSplit.size() != 0) result.push_back(toSplit);
}


string StringUtil::trim(string in)
{
    // Remove leading spaces
    while(in.find(" ") == 0)
    {
        in = in.substr(1); // chop off first char
    }

    // Remove trailing spaces
    // Special case empty string to avoid infinite loop
    // (if size = 0, then size-1=-1, which is the same as if space is "not found")
    while(in.find_last_of(" ") == in.size()-1 && in.size() != 0)
    {
        in = in.substr(0,in.size()-1); // chop off last char
    }

    return in;
}
