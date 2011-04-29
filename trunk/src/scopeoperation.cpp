#include <iostream>

#include "scopeoperation.h"
#include "stringutil.h"

using namespace std;

ScopeOperation::ScopeOperation(string arg)
{
    // Default values
    m_type = UNKNOWN;
    m_params.data[0] = 0.0;
    m_params.data[1] = 0.0;
    m_params.data[2] = 0.0;
    m_params.data[3] = 1.0;

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
        m_params.data[i] = strtod(params[i].c_str(), NULL);
    }
}

ScopeOperation::~ScopeOperation() { }

void ScopeOperation::evaluate(Feature* feat)
{
    // Edit the scope of the input feature according to this operation type
    switch (m_type)
    {
        case SCALE:
            feat->getScope()->setSize(m_params);
            break;

        case ROTATE:
            feat->getScope()->rotateX(m_params.x);
            feat->getScope()->rotateY(m_params.y);
            feat->getScope()->rotateZ(m_params.z);
            break;

        case TRANSLATE:
            feat->getScope()->translate(m_params);
            break;

        case UNKNOWN: // fall-through
        default:
            break;
    }
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

    cout << "(" << m_params.x << ", " << m_params.y << ", " << m_params.z << ")";
}
