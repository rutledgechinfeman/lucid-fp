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
    Rule();

    virtual ~Rule();

    virtual bool evaluateCondition();
    virtual bool apply(Feature target);

private:
    string m_id;
    Condition m_condition;
    GrammarNode m_gnode;
};

#endif // RULE_H
