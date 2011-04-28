#ifndef OPERATIONNODE_H
#define OPERATIONNODE_H

#include "grammarnode.h"
#include "operation.h"
#include <string>

#define NONE 0
#define COMP 100
#define SUBDIV 101
#define REPEAT 102

/// Parameters to an operation may be relative or not, convenience struct to record it
struct argument
{
    argument(double v, bool r = false) { value = v; relative = r; }

    double value;
    bool relative;
};

class OperationNode : public GrammarNode
{
public:
    /// CTOR
    OperationNode(string arg = "");

    /// Operate on the feature's scope, add to it's children as applicable
    void evaluate(Feature* feat, Factory &fac);

    /// Debug purposes only
    virtual void printSelf();

private:
    /// Parse any operation (symbol, opnode, op)
    void parseOp(string line);

    /// Parse an argument, determine its value and whether it is relative
    argument genArg(string argString);

    /// What kind of operation, ie: comp, subdiv, repeat
    int m_type;

    /// First parameter: always a string
    string m_stringArg;

    /// 2nd+ parameters: numbers, possibly relative
    vector<argument> m_otherArgs;

    /// Child operations that apply only to the scope, ie S, T, R
    vector<Operation*> m_operations;
};

#endif // OPERATIONNODE_H
