#ifndef RULE_H
#define RULE_H

class Rule
{
public:
    Rule();
    Rule(string id, Condition c, GrammarNode gn)

    virtual ~Rule();

    virtual bool evaluateCondition();
    virtual bool apply(Feature target);

private:
    string m_id;
    Condition m_condition;
    GrammarNode m_gnode;
};

#endif // RULE_H
