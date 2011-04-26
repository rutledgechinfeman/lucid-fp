#include "rule.h"

Rule::Rule(string pred, GrammarNode suc, Condition con)
{
    m_id = pred;
    m_successor = suc;
    m_condition = con;
}

Rule::~Rule(){}

bool Rule::evaluateCondition(Feature &f)
{
    return m_condition.evaluate(f);
}

bool Rule::apply(Feature target)
{
    (void) target; // TODO

    return false;
}
