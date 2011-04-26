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

    void addRule(string pred, Rule);

private:
    map< string, vector<Rule> > m_ruleMap;
};

#endif // GRAMMAR_H
