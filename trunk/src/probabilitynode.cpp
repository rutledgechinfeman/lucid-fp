#include "probabilitynode.h"
#include "stringutil.h"

#include <vector>

#include <iostream>
using namespace std;

ProbabilityNode::ProbabilityNode(string line)
{

    cout << line << endl;

    vector<string> rules;
    StringUtil::split(line, "~", rules);
    string rule, probString;
    for (int i = 0; i < rules.size(); i ++) {
        rule = StringUtil::trim(rules[i]);

        probString = StringUtil::trim(rule.substr(rule.find(":")));


    }



}

ProbabilityNode::~ProbabilityNode()
{
}
