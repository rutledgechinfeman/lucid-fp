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
    Rule(string pred, GrammarNode suc, Condition con = Condition());

    virtual ~Rule();

    virtual bool evaluateCondition();
    virtual bool apply(Feature target);

private:
    string m_id;
    Condition m_condition;
    GrammarNode m_successor;
};

#endif // RULE_H
