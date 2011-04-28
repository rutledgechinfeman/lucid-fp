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


void ProbabilityNode::evaluate(Feature* feat, Factory &fac) {


}

void ProbabilityNode::printSelf()
{
    cout << "ProbabilityNode [";
    for( int i = 0 ; i < m_children.size() ; ++ i )
    {
        cout << "p=" << m_probabilities[i];
        m_children[i]->printSelf();
    }
    cout << "]" << endl;
}
