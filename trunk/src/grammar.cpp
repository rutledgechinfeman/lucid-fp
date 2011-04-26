#include "grammar.h"
#include <iostream>

using namespace std;

Grammar::Grammar() {


}

Grammar::~Grammar() { }


void Grammar::addRule(string pred, Rule rule) {

    if (m_ruleMap.find(pred) == m_ruleMap.end()) {
        m_ruleMap[pred] = vector<Rule>();
    }

    m_ruleMap[pred].push_back(rule);
}
