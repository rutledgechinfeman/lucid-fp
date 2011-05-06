#include <iostream>

#include "scopeoperation.h"
#include "stringutil.h"

using namespace std;

ScopeOperation::ScopeOperation(string arg)
{
    // Default values
    m_type = UNKNOWN;

    // Parse type
    string type = StringUtil::trim(arg.substr(0, arg.find("(")));
    if      (type == "S") { m_type = SCALE;     }
    else if (type == "R") { m_type = ROTATE;    }
    else if (type == "T") { m_type = TRANSLATE; }
    else { cerr << "ERROR: Unrecognized operation [" << type << "] in: " << arg << endl; }

    // Separate out params
    int paramStart = arg.find("(") + 1;
    int paramEnd = arg.find(")");
    string paramString = StringUtil::trim(arg.substr(paramStart, paramEnd - paramStart ));
    vector<string> params;
    StringUtil::split(paramString, ",", params, false, "[", "]");

    // Parse each param
    if (params.size() != 3) { cerr << "ERROR: Invalid number of params in operation line: " << arg << endl; }
    for (unsigned int i = 0; i < params.size(); ++ i)
    {
        // Random interval
        if (params[i].find("[") != string::npos)
        {
            vector<string> randParams;
            StringUtil::split(params[i].substr(1, params[i].size() - 2), ",", randParams);
            data[i] = OpVal(strtod(randParams[0].c_str(), NULL), strtod(randParams[1].c_str(), NULL));
        }

        // Relative value
        else if (params[i].find("Scope") != string::npos)
        {
            bool neg = (params[i].find("-") != string::npos);
            if      (params[i].find("X") != string::npos) { data[i] = OpVal(X, neg); }
            else if (params[i].find("Y") != string::npos) { data[i] = OpVal(Y, neg); }
            else if (params[i].find("Z") != string::npos) { data[i] = OpVal(Z, neg); }
            else
            {
                cerr << "ERROR: Received relative Scope input for a scope operation, but no valid dimension is present, defaulting to X: " << params[i] << endl;
                data[i] = OpVal(X);
            }
        }

        // Normal single number param
        else
        {
            data[i] = OpVal(strtod(params[i].c_str(), NULL));
        }
    }
}

ScopeOperation::~ScopeOperation() { }

Scope ScopeOperation::evaluate(Scope &in)
{
    Scope toReturn = in.copy();

    Vector4 rando;
    rando.x = data[X].getValue(toReturn);
    rando.y = data[Y].getValue(toReturn);
    rando.z = data[Z].getValue(toReturn);
    rando.w = 1.0;

    // Edit the scope of the input feature according to this operation type
    switch (m_type)
    {
        case SCALE:
            toReturn = toReturn.setScale(rando);
            break;

        case ROTATE:
            toReturn = toReturn.rotateX(rando.x);
            toReturn = toReturn.rotateY(rando.y);
            toReturn = toReturn.rotateZ(rando.z);
            break;

        case TRANSLATE:
            toReturn = toReturn.translate(rando);
            break;

        case UNKNOWN: // fall-through
        default:
            break;
    }

    return toReturn;
}

void ScopeOperation::printSelf()
{
    cout << "Operation_";

    switch (m_type)
    {
        case SCALE:
            cout << "S";
            break;

        case ROTATE:
            cout << "R";
            break;

        case TRANSLATE:
            cout << "T";
            break;

        case UNKNOWN: // fall-through
        default:
            cout << "?";
            break;
    }
}
