#include "probabilitynode.h"
#include "stringutil.h"
#include "operation.h"
#include "symbol.h"
#include <vector>
#include <iostream>
#include <stdlib.h>
#include <math.h>

using namespace std;

ProbabilityNode::ProbabilityNode(string line)
{

    cout << line << endl;

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
            cout << "Operation: " << arg << endl;
            prospectiveChild = new Operation(arg);
        }
        else
        {
            cout << "Symbol: " << arg << endl;
            prospectiveChild = new Symbol(arg);
        }
        m_children.push_back(prospectiveChild);

        probString = StringUtil::trim(rule.substr(rule.find(":")));
        m_probabilities.push_back(atof(probString.c_str()));
    }
}

ProbabilityNode::~ProbabilityNode()
{
}
