#include "operationnode.h"
#include "symbol.h"
#include <iostream>
#include <vector>
#include "stringutil.h"

using namespace std;

OperationNode::OperationNode(string arg)
{
    m_type = NONE; // default value

    // Split this operation into several subtypes
    vector<string> ops;
    StringUtil::split(arg, "_", ops, false, "<", ">");

    // Parse each one
    string op;
    for (unsigned int i = 0; i < ops.size(); i ++)
    {
        op = StringUtil::trim(ops[i]);
        parseOp(op, i);
    }
}

OperationNode::~OperationNode() { }

void OperationNode::evaluate(Feature* feat, Factory &fac, Scope scope)
{
    // If childIndices is greater than 0 then this operation node has only scope ops and
    // symbols as children. We just execute the scope ops, and evaluate the symbol children
    // in the proper order
    if (m_childIndices.size() > 0)
    {
        int childIndex = 0;
        int opIndex = 0;
        for (unsigned int i = 0; i < m_childIndices.size() + m_opIndices.size(); ++ i)
        {
            if (m_childIndices.find(i) != m_childIndices.end())
            {
                m_children[childIndex]->evaluate(feat, fac, scope);
                childIndex++;
            }
            else if (m_opIndices.find(i) != m_opIndices.end())
            {
                scope = m_operations[opIndex]->evaluate(scope);
                opIndex++;
            }
            else { cerr << "ERROR: Index somehow not contained in either ops or children" << endl; }
        }

        return;
    }

    // Otherwise, this is an operation in and of itself, and we have to evaluate it
    if (m_type == COMP)
    {
        if(m_stringArg != "sidefaces") { cerr << "ERROR: We only support the comp operation on 'sidefaces', not: " << m_stringArg << endl; return; }

        // Flatten side faces
        scope = scope.setScaleComponent(0.0, 2);

        for (int i = 0 ; i < 4 ; ++i)
        {
            // Move it along X basis
            scope = scope.translate(scope.getXBasis() * scope.getScale().x);

            // Negate the X basis
            scope = scope.setBasisComponent(0,-scope.getXBasis());

            // Swap X and Z basis
            Vector4 temp = scope.getBasisComponent(0);
            scope = scope.setBasisComponent(0, scope.getBasisComponent(2));
            scope = scope.setBasisComponent(2, temp);

            // Evaluate
            m_children[0]->evaluate(feat, fac, scope);
        }
    }
    else if (m_type == SUBDIV)
    {
        // Figure out what axis we're repeating on
        int index = -1;
        if      (m_stringArg == "X") { index = 0; }
        else if (m_stringArg == "Y") { index = 1; }
        else if (m_stringArg == "Z") { index = 2; }
        else { cerr << "ERROR: Unrecognized string argument in Subdiv operation: " << m_stringArg << endl; return; }

        // Sum (non-)relative repeat lengths
        double rel = 0.0;
        double notrel = 0.0;
        for (unsigned int i = 0 ; i < m_otherArgs.size(); ++ i)
        {
            if (m_otherArgs[i].relative) { rel    += m_otherArgs[i].value; }
            else                         { notrel += m_otherArgs[i].value; }
        }

        double dimLen = scope.getScale().data[index];
        double remainingSpace = dimLen - notrel;
        double relativeUnit = remainingSpace / rel;

        // Insert children based on calculated size
        for (unsigned int i = 0; i < m_children.size(); ++ i)
        {
            // TODO this could be cleaned up
            if (m_otherArgs[i].relative)
            {
                scope = scope.setScaleComponent(m_otherArgs[i].value * relativeUnit, index);
                m_children[i]->evaluate(feat, fac, scope);
                scope = scope.translate(scope.getBasisComponent(index)  * m_otherArgs[i].value * relativeUnit);
            }
            else
            {
                scope = scope.setScaleComponent(m_otherArgs[i].value, index);
                m_children[i]->evaluate(feat, fac, scope);
                scope = scope.translate(scope.getBasisComponent(index)  * m_otherArgs[i].value);
            }
        }
    }
    else if (m_type == REPEAT)
    {
        // Figure out what axis we're repeating on
        int index = -1;
        if      (m_stringArg == "X") { index = 0; }
        else if (m_stringArg == "Y") { index = 1; }
        else if (m_stringArg == "Z") { index = 2; }
        else { cerr << "ERROR: Unrecognized string argument in Repeat operation: " << m_stringArg << endl; return; }

        // Calculate repeat specifications
        double dimLen = scope.getScale().data[index];
        int numRepeats = dimLen / m_otherArgs[0].value;
        double repeatLength = dimLen / ((double) numRepeats);

        // Get a new scope to translate for each child
        Scope newScope = scope.setScaleComponent(repeatLength, index);
        Vector4 transVec = scope.getBasisComponent(index) * repeatLength;

        for (int i = 0; i < numRepeats; i ++)
        {
            // Pass the new scope down, and translate it for the next one
            m_children[0]->evaluate(feat, fac, newScope);
            newScope = newScope.translate(transVec);
        }
    }
}

void OperationNode::parseOp(string line, int index)
{
    /*
     * Pick the child-type based on which characters are present
     */
    // Symbol: has no parens
    if (line.find("(") == string::npos)
    {
        m_children.push_back(new Symbol(line));
        m_childIndices.insert(index);
        return;
    }

    // Operation: has parens but no braces
    if (line.find("{") == string::npos)
    {
        m_operations.push_back(new ScopeOperation(line));
        m_opIndices.insert(index);
        return;
    }

    // OperationNode: has parens and braces

    /*
     * Parse type
     */
    string opType = line.substr(0, line.find("("));
    if      (opType == "Comp")   { m_type = COMP;   }
    else if (opType == "Subdiv") { m_type = SUBDIV; }
    else if (opType == "Repeat") { m_type = REPEAT; }
    else { cerr << "ERROR: Ignoring unrecognized operation: " << line << endl; return; }

    /*
     * Parse parameters
     */
    string params = line.substr(line.find("(") + 1, line.find(")") - line.find("(") - 1);
    vector<string> paramList;
    StringUtil::split(params, ",", paramList);
    if (paramList.size() <= 0) { cerr << "ERROR: No parameters specified for operation: " << line << endl; return; }

    // The first parameter is always a string
    m_stringArg = StringUtil::trim(paramList[0]);

    // The rest are doubles or doubles followed by an r
    for (unsigned int i = 1; i < paramList.size(); i ++)
    {
        m_otherArgs.push_back(genArg(StringUtil::trim(paramList[i])));
    }

    /*
     * Parse targets
     */
    string targetString = line.substr(line.find("{") + 1, line.find_last_of("}") - line.find("{") - 1);
    vector<string> targets;
    StringUtil::split(targetString, "|", targets, false, "{", "}");

    string target;
    for (unsigned int i = 0; i < targets.size(); i ++)
    {
        target = StringUtil::trim(targets[i]);

        if (target.find("(") != string::npos) { m_children.push_back(new OperationNode(target)); }
        else { m_children.push_back(new Symbol(target)); }
    }
}

argument OperationNode::genArg(string argString)
{
    if (argString.find("r") != string::npos)
    {
        return argument(atof(argString.substr(0, argString.find("r")).c_str()), true);
    }
    else
    {
        return argument(atof(argString.c_str()));
    }
}

void OperationNode::printSelf()
{
    cout << "OperationNode_";

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
        case NONE : // fall-through
        default:
            cout << "notype";
    }

    cout << "(" << m_stringArg;
    for (unsigned int i = 0; i < m_otherArgs.size(); i ++) { cout << ", " << m_otherArgs[i].value; }

    cout << ")[ <Operations: (";
    for( unsigned int i = 0 ; i < m_operations.size() ; ++ i)
    {
        if( i != 0 ) cout << ", ";
        m_operations[i]->printSelf();
    }

    cout << "), Children: (";
    for( unsigned int i = 0 ; i < m_children.size() ; ++ i )
    {
        if( i != 0 ) cout << ", ";
        m_children[i]->printSelf();
    }

    cout << ")> ]";
}

