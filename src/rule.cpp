#include "rule.h"

Rule::Rule(string pred, GrammarNode suc, Condition con)
{
    m_id = pred;
    m_successor = suc;
    m_condition = con;
}

Rule::~Rule(){}

bool Rule::evaluateCondition()
{
    return m_condition.evaluate();
}

bool Rule::apply(Feature target)
{
    return false;
}
