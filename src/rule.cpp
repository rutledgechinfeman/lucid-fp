#include "rule.h"

Rule::Rule()
{
}

Rule::~Rule(){}

bool Rule::evaluateCondition(){return false;}
bool Rule::apply(Feature target){return false;}
