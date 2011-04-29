#include <iostream>

#include "scopeoperation.h"
#include "stringutil.h"

using namespace std;

ScopeOperation::ScopeOperation(string arg)
{
    // Default values
    m_type = UNKNOWN;
    m_params[0] = 0.0;
    m_params[1] = 0.0;
    m_params[2] = 0.0;

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
    StringUtil::split(paramString, ",", params);

    // Parse each param
    if (params.size() != 3) { cerr << "ERROR: Invalid number of params in operation line: " << arg << endl; }
    for (unsigned int i = 0; i < params.size(); ++ i)
    {
        m_params[i] = strtod(params[i].c_str(), NULL);
    }
}

ScopeOperation::~ScopeOperation() { }

void ScopeOperation::evaluate(Feature* feat, Factory &fac)
{
    // TODO
}

void ScopeOperation::printSelf()
{
    cout << "Operation_";

    switch(m_type)
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
        case UNKNOWN:
            cout << "?";
            break;
    }

    cout << "(" << m_params[0] << ", " << m_params[1] << ", " << m_params[2] << ")";
}
