#ifndef SYMBOL_H
#define SYMBOL_H

#include "grammarnode.h"
#include <vector>
#include <string>

class Symbol : public GrammarNode
{
public:
    Symbol(string arg = "", string type = "");
    virtual ~Symbol();

    void evaluate(Feature* feat, Factory &fac, Scope scope);
    virtual void printSelf();

private:
    string m_symbol;
    int m_faces;
    string m_type;
};

#endif // SYMBOL_H
