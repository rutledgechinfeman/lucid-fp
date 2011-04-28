#ifndef GRAMMAR_H
#define GRAMMAR_H

#include <string>
#include <map>
#include <vector>
#include "rule.h"

using std::string;
using std::map;


class Grammar
{
public:
    Grammar();
    virtual ~Grammar();

    virtual void addRule(string pred, Rule rule);
    virtual Rule lookupRule(string pred);

private:
    map< string, vector<Rule> > m_ruleMap;
};

#endif // GRAMMAR_H
