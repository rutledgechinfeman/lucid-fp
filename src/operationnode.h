#ifndef OPERATIONNODE_H
#define OPERATIONNODE_H

#include <string>
#include <set>

#include "grammarnode.h"
#include "scopeoperation.h"

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

    /// DTOR
    virtual ~OperationNode();

    /// Operate on the feature's scope, add to it's children as applicable
    void evaluate(Feature* feat, Factory &fac, Scope scope);

    /// Debug purposes only
    virtual void printSelf();

private:
    /// Parse any operation (symbol, opnode, op)
    void parseOp(string line, int index);

    /// Parse an argument, determine its value and whether it is relative
    argument genArg(string argString);

    /// What kind of operation, ie: comp, subdiv, repeat
    int m_type;

    /// First parameter: always a string
    string m_stringArg;

    /// 2nd+ parameters: numbers, possibly relative
    vector<argument> m_otherArgs;

    /// Child operations that apply only to the scope, ie S, T, R
    vector<ScopeOperation*> m_operations;

    /// Helper for keeping track of in what order we perform operations
    set<int> m_opIndices;

    /// Helper for keeping track of in what order we perform operations
    set<int> m_childIndices;
};

#endif // OPERATIONNODE_H
