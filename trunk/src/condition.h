#ifndef CONDITION_H
#define CONDITION_H

#include <string>

using std::string;

class Condition
{
public:
    Condition();
    Condition(string line);
    virtual ~Condition();

    virtual bool evaluate();
};

#endif // CONDITION_H
