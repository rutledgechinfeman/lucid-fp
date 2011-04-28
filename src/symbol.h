#ifndef SYMBOL_H
#define SYMBOL_H

#include "grammarnode.h"
#include <vector>

class Symbol : public GrammarNode
{
public:
    Symbol(string arg = "");
    virtual ~Symbol();
};

#endif // SYMBOL_H
