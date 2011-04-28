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

Rule* Grammar::lookupRule(Feature* parent)
{
    // Get the predecessor symbol
    string pred = parent->getSymbol();

    // Make sure rules exist for this predecessor
    if(m_ruleMap.find(pred) == m_ruleMap.end())
    {
        cerr << "ERROR: Rule requested for symbol [" << pred << "] but there are none." << endl;
        return NULL;
    }

    // Pick the first rule that can be applied
    const vector<Rule*>& rules = m_ruleMap[pred];
    for(unsigned int i = 0; i < rules.size(); ++i)
    {
        if(rules[i]->evaluateCondition(*parent))
        {
            return rules[i];
        }
    }

    cerr << "ERROR: Rule requested for symbol [" << pred << "] and no conditions evaluated to true." << endl;
    return NULL;
}
