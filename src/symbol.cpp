#include "symbol.h"
#include "feature.h"
#include "factory.h"

Symbol::Symbol(string arg, string type)
{
    m_symbol = arg;
    m_type = type;
    m_faces = atoi(type.c_str());
}

Symbol::~Symbol() { }

void Symbol::evaluate(Feature* feat, Factory &fac, Scope scope)
{
    Feature* toAdd = fac.instanceOf(m_symbol, scope);
    if (m_faces > 0 || m_type != "") { toAdd->setMassModel(m_faces, m_type); }
    feat->addChild(toAdd);
    feat->setActive(false);
}

void Symbol::printSelf()
{
    cout << "Symbol[ " << m_symbol << " ]";
}

