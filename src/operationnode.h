#ifndef OPERATIONNODE_H
#define OPERATIONNODE_H

#include "grammarnode.h"
#include "operation.h"
#include <string>

#define NONE 0
#define COMP 100
#define SUBDIV 101
#define REPEAT 102


struct argument {

    argument(double v, bool r = false) { value = v; relative = r; }

    double value;
    bool relative;

};


class OperationNode : public GrammarNode
{
public:
    OperationNode(string arg = "");

    void evaluate(Feature* feat, Factory &fac);
    virtual void printSelf();

private:
    void parseOp(string line);

    int m_type;
    vector<Operation*> m_operations;

    string m_stringArg;
    vector<argument> m_otherArgs;


    argument genArg(string argString);
};

#endif // OPERATIONNODE_H
