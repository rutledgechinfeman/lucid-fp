#include "probabilitynode.h"
#include "stringutil.h"
#include "operationnode.h"
#include "symbol.h"
#include <vector>
#include <iostream>
#include <stdlib.h>
#include <math.h>

using namespace std;

ProbabilityNode::ProbabilityNode(string line)
{

    vector<string> rules;
    StringUtil::split(line, "~", rules);
    string rule, probString;
    for (unsigned int i = 0; i < rules.size(); i ++)
    {
        rule = StringUtil::trim(rules[i]);

        GrammarNode* prospectiveChild;
        string arg = StringUtil::trim(rule.substr(0, rule.find(":")));
        if (rule.find("(") != string::npos)
        {
            prospectiveChild = new OperationNode(arg);
        }
        else
        {
            prospectiveChild = new Symbol(arg);
        }
        m_children.push_back(prospectiveChild);

        probString = StringUtil::trim(rule.substr(rule.find(":") + 1));
        m_probabilities.push_back(atof(probString.c_str()));
    }
}

ProbabilityNode::~ProbabilityNode()
{
}


void ProbabilityNode::evaluate(Feature* feat, Factory &fac, Scope scope)
{
    double sample = (double) rand() / (double) RAND_MAX; // random number between 0 and 1 (found this online)
    double total = 0;
    unsigned int i;
    for (i = 0; i < m_probabilities.size(); ++ i)
    {
        total += m_probabilities[i];
        if( total >= sample )
        {
            m_children[i]->evaluate(feat, fac, scope);
            break;
        }
    }
    feat->setActive(false); // just in case

    // Check if we ran off the end, warn if so
    if (i == m_probabilities.size()) { cerr << "ERROR: Probability node didn't pick a rule! Probability was: " << sample << endl; }
}

void ProbabilityNode::printSelf()
{
    cout << "ProbabilityNode[";
    for( unsigned int i = 0 ; i < m_children.size() ; ++ i )
    {
        cout << " p=" << m_probabilities[i] << ":";
        m_children[i]->printSelf();
    }
    cout << " ]";
}
