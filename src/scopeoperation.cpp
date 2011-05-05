#include <iostream>

#include "scopeoperation.h"
#include "stringutil.h"
#include <stdlib.h>

using namespace std;

ScopeOperation::ScopeOperation(string arg)
{
    // Default values
    m_type = UNKNOWN;
    m_hiParams.data[0] = 0.0;
    m_hiParams.data[1] = 0.0;
    m_hiParams.data[2] = 0.0;
    m_hiParams.data[3] = 1.0;
    m_loParams.data[0] = 0.0;
    m_loParams.data[1] = 0.0;
    m_loParams.data[2] = 0.0;
    m_loParams.data[3] = 1.0;

    m_atAllRandom = false;

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
        // Straight number param
        double high, low;
        if (params[i].find("[") == string::npos) {
            high = strtod(params[i].c_str(), NULL);
            low = high;
        }

        // Random interval
        else
        {
            vector<string> randParams;
            StringUtil::split(params[i].substr(1, params[i].size() - 2), ",", randParams);

            high = strtod(randParams[1].c_str(), NULL);
            low = strtod(randParams[0].c_str(), NULL);

            m_atAllRandom = true; //we have randomness!
        }

        m_hiParams.data[i] = high;
        m_loParams.data[i] = low;
    }
}

ScopeOperation::~ScopeOperation() { }

Scope ScopeOperation::evaluate(Scope &in)
{
    Scope toReturn = in.copy();

    Vector4 rando;

    if (m_atAllRandom) {
        rando = Vector4((double)rand(), (double)rand(), (double)rand(), RAND_MAX);
        rando /= (double) RAND_MAX;

        for (int i = 0; i < 3; i ++) {

            rando.data[i] *= m_hiParams.data[i] - m_loParams.data[i];
            rando.data[i] += m_loParams.data[i];
        }
    }
    else {
        rando = m_hiParams;
    }


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

    cout << "(" << m_hiParams.x << ", " << m_hiParams.y << ", " << m_hiParams.z << ")" << endl;
}
