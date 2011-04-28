#include "symbol.h"
#include "feature.h"
#include "factory.h"

Symbol::Symbol(string arg)
{
    m_symbol = arg;
}

Symbol::~Symbol() { }


void Symbol::evaluate(Feature* feat, Factory &fac) {

    feat->setActive(false);

    feat->addChild(fac.instanceOf(m_symbol));

}
