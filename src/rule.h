#ifndef RULE_H
#define RULE_H

#include <string>
#include "condition.h"
#include "grammarnode.h"
#include "feature.h"

using std::string;

class Rule
{
public:
    Rule(string pred = "", GrammarNode* suc = NULL, Condition con = Condition());

    virtual ~Rule();

    virtual bool evaluateCondition(Feature &f);
    virtual bool apply(Feature* target, Factory& fac, Scope scope);

private:
    string m_id;
    Condition m_condition;
    GrammarNode* m_successor;
};

#endif // RULE_H
