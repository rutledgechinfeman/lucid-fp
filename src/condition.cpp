#include "condition.h"
#include "stringutil.h"
#include <iostream>
#include <queue>

using namespace std;

Condition::Condition(string line)
{
    // Set default values
    m_type   = UNKNOWN;
    m_target = UNKNOWN;
    m_param  = UNKNOWN;
    m_rhs    = UNKNOWN;

    // Default case, null condition which always evaluates to true
    if (line == "NULL")
    {
        m_type = EMPTY;
        return;
    }

    // Get the target (ie, in Shape.visible(street) = none, it's Shape)
    string target = StringUtil::trim(line.substr(0, line.find(".")));
    if      (target == "Shape") { m_target = SHAPE; }
    else if (target == "Scope") { m_target = SCOPE; }
    line = StringUtil::trim(line.substr(line.find(".") + 1)); // cut off target and trailing .

    // Get the type (ie, in Shape.visible(street) = none, it's visible)
    string type = StringUtil::trim(line.substr(0, line.find("(")));
    if      (type == "occluded") { m_type = OCCLUDED; }
    else if (type == "visible")  { m_type = VISIBLE;  }
    line = StringUtil::trim(line.substr(line.find("(") + 1)); // cut off type and trailing (

    // Get the param (ie, in Shape.visible(street) = none, it's street)
    string param = StringUtil::trim(line.substr(0, line.find(")")));
    if      (param == "all")      { m_param = ALL;      }
    else if (param == "noparent") { m_param = NOPARENT; }
    else if (param == "street")   { m_param = STREET;   }
    line = StringUtil::trim(line.substr(line.find(")") + 1)); // cut off param and trailing )

    // Get the rhs (ie, in Shape.visible(street) = none, it's none)
    string rhs = StringUtil::trim(line.substr(line.find("=") + 1));
    if      (rhs == "none") { m_rhs = NONE; }
    else if (rhs == "part") { m_rhs = PART; }
    else if (rhs == "full") { m_rhs = FULL; }
}

Condition::~Condition()
{
}

bool Condition::evaluate(Feature &f)
{
    bool output = true;
    Feature *root, *feat, *parent;
    queue<Feature*> q;
    bool parentOccluded, otherOccluded;
    int occTest;

    switch (m_type)
    {
        case EMPTY:
            output = true;
            break;

        case VISIBLE:
            if (m_param == STREET) { return f.getScope().getBasisComponent(0).x > 0; }
            else { cerr << "WARNING: Visibility condition only supported for STREET, and not #: " << m_param << endl; }
            break;

        case OCCLUDED:

            root = &f; //get the root of the feature tree
            parent = f.getParent();
            while (root->getParent() != NULL) {
                root = root->getParent();
            }

            occTest = parent->getScope().occludes(f.getScope());
            cout << "RESULT: " << occTest << endl;
            if (occTest > 0) {


                if (m_param == ALL) {
                    output = false;
                }

                parentOccluded = true;
            }

            if (output) {
                q.push(root);
                while (!q.empty()) {

                    feat = q.front();
                    q.pop();

                    if (feat->getActive() && feat!= &f) {

                        occTest = feat->getScope().occludes(f.getScope());
                        cout << "RESULT: " << occTest << endl;
                        if (occTest > 0) {

                            otherOccluded = true;
                            break;
                        }
                    }

                    if (feat != &f && feat != parent) {
                        for (int i = 0; i < feat->getNumChildren(); i ++) {
                            q.push(feat->getChild(i));
                        }
                    }

                }
            }



            if (m_param == NOPARENT && (otherOccluded)) {
                output = false;
            }


            break;


        default:
            cerr << "ERROR: Unknown condition type: " << m_type << endl;
            output = false;
    }

    return output;
}
