#include "grammar.h"
#include <iostream>

using namespace std;

Grammar::Grammar() {


}

Grammar::~Grammar()
{
    // TODO: delete rules
}


void Grammar::addRule(string pred, Rule* rule)
{
    // Lazily initialize a rule vector for each id
    if (m_ruleMap.find(pred) == m_ruleMap.end())
    {
        m_ruleMap[pred] = vector<Rule*>();
    }

    // Register this rule with its respective id
    m_ruleMap[pred].push_back(rule);
}

Rule* Grammar::lookupRule(string pred)
{
    if(m_ruleMap.find(pred) == m_ruleMap.end())
    {
        return NULL;
    }
    else
    {
        // TODO: pick a rule, somehow...

        return new Rule(); // we won't new this when we do it for real
    }
}
