#include "rule.h"

Rule::Rule(string pred, GrammarNode* suc, Condition con)
{
    m_id = pred;
    m_successor = suc;
    m_condition = con;
}

Rule::~Rule()
{
    delete m_successor;
}

bool Rule::evaluateCondition(Feature &f)
{
    return m_condition.evaluate(f);
}

bool Rule::apply(Feature* target, Factory& fac)
{
    if (!m_successor) { return false; }

    m_successor->evaluate(target, fac);

    return true;
}
