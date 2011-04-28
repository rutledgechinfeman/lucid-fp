#ifndef SYMBOL_H
#define SYMBOL_H

#include "grammarnode.h"
#include <vector>
#include <string>

class Symbol : public GrammarNode
{
public:
    Symbol(string arg = "");
    virtual ~Symbol();

    void evaluate(Feature* feat, Factory &fac);
    virtual void printSelf();

private:
    string m_symbol;
};

#endif // SYMBOL_H
