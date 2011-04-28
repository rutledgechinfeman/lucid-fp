#ifndef GRAMMAR_H
#define GRAMMAR_H

#include "feature.h"
#include "rule.h"

#include <map>
#include <string>
#include <vector>

using std::string;
using std::map;

class Grammar
{
public:
    /// CTOR
    Grammar();

    /// DTOR
    virtual ~Grammar();

    /// Insert to the table
    virtual void addRule(string pred, Rule* rule);

    /// Get from the table
    virtual Rule* lookupRule(Feature* parent);

private:
    /// Master rule table
    map< string, vector<Rule*> > m_ruleMap;
};

#endif // GRAMMAR_H
