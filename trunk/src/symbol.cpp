#include "symbol.h"
#include "feature.h"
#include "factory.h"

Symbol::Symbol(string arg)
{
    m_symbol = arg;
}

Symbol::~Symbol() { }


void Symbol::evaluate(Feature* feat, Factory &fac, Scope scope)
{
    feat->addChild(fac.instanceOf(m_symbol, scope));
    feat->setActive(false);
}

void Symbol::printSelf()
{
    cout << "Symbol[ " << m_symbol << " ]";
}

