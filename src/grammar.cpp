#include "grammar.h"
#include <iostream>

using namespace std;

Grammar::Grammar() {


}

Grammar::~Grammar() { }


void Grammar::addRule(string pred, Rule rule)
{
    // Lazily initialize a rule vector for each id
    if (m_ruleMap.find(pred) == m_ruleMap.end())
    {
        m_ruleMap[pred] = vector<Rule>();
    }

    // Register this rule with its respective id
    m_ruleMap[pred].push_back(rule);
}

Rule Grammar::lookupRule(string pred)
{
    if(m_ruleMap.find(pred) == m_ruleMap.end())
    {
        return Rule();
    }
    else
    {
        // TODO: pick a rule, somehow...

        return Rule();
    }
}
