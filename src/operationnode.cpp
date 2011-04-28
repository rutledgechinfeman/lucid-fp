#include "operationnode.h"
#include "symbol.h"
#include "operation.h"
#include <iostream>
#include <vector>
#include "stringutil.h"

using namespace std;

OperationNode::OperationNode(string arg)
{
    vector<string> ops;
    StringUtil::split(arg, "_", ops);

    m_type = NONE;

    string op;
    for (int i = 0; i < ops.size(); i ++)
    {
        op = StringUtil::trim(ops[i]);
        parseOp(op);
    }

}

void OperationNode::evaluate(Feature* feat, Factory &fac)
{

}

void OperationNode::parseOp(string line)
{
    GrammarNode* prospectiveChild = NULL;
    Operation* prospectiveOperation = NULL;

    //see if this is an operation or just a symbol
    if (line.find("(") != string::npos)
    {
        if (line.find("{") != string::npos) {

            string opType = line.substr(0, line.find("("));

            if      (opType == "Comp")      { m_type = COMP; }
            else if (opType == "Subdiv")    { m_type = SUBDIV; }
            else if (opType == "Repeat")    { m_type = REPEAT; }
            else {
                cerr << "Unrecognized operation: " << line << endl;
                return;
            }

            //get the parameters of the operation
            string params = line.substr(line.find("(") + 1, line.find(")") - line.find("(") - 1);


            vector<string> paramList;
            StringUtil::split(params, ",", paramList);

            if (paramList.size() <= 0) {
                cerr << "no parameters specified for operation: " << line << endl;
                return;
            }

            m_stringArg = StringUtil::trim(paramList[0]);

            //make the list of double parameters
            for (int i = 1; i < paramList.size(); i ++) {
                m_otherArgs.push_back(genArg(StringUtil::trim(paramList[i])));
            }

            //get the targets of the operation
            string targetString = line.substr(line.find("{") + 1, line.find_last_of("}") - line.find("{") - 1);
            vector<string> targets;
            StringUtil::split(targetString, ",", targets);

            string target;
            for (int i = 0; i < targets.size(); i ++) {
                target = StringUtil::trim(targets[i]);

                if (target.find("(") != string::npos) {
                    m_children.push_back(new OperationNode(target));
                }
                else {
                    m_children.push_back(new Symbol(target));
                }

            }




        }

        else {
            prospectiveOperation = new Operation(line);
        }

    }
    else
    {
        prospectiveChild = new Symbol(line);
    }

    if (prospectiveChild != NULL) {
        m_children.push_back(prospectiveChild);
    }
    if (prospectiveOperation != NULL) {
        m_operations.push_back(prospectiveOperation);
    }

}

argument OperationNode::genArg(string argString)
{
    if (argString.find("r") != string::npos) {
        return argument(atof(argString.substr(0, argString.find("r")).c_str()), true);
    }
    else {
        return argument(atof(argString.c_str()));
    }
}


void OperationNode::printSelf()
{
    cout << "OperationNode ";

    switch(m_type)
    {
    case COMP :
        cout << "comp";
        break;
    case SUBDIV :
        cout << "subdiv";
        break;
    case REPEAT :
        cout << "repeat";
        break;
    case NONE :
    default:
        cout << "notype";
    }

    cout << "(";

    cout << m_stringArg;

    for (int i = 0; i < m_otherArgs.size(); i ++) {
        cout << ", " << m_otherArgs[i].value;
    }
    cout << ")";

    cout << " [";

    cout << "Operations: (";
    for( int i = 0 ; i < m_operations.size() ; ++ i)
    {
        m_operations[i]->printSelf();
        cout << ", ";
    }
    cout << ")";

    cout << "Children:";
    for( int i = 0 ; i < m_children.size() ; ++ i )
    {
        m_children[i]->printSelf();
    }
    cout << "]" << endl;
}

