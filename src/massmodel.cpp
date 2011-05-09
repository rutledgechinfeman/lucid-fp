#include "massmodel.h"
#include "feature.h"
#include "grammarnode.h"
#include "factory.h"
#include "scope.h"

MassModel::MassModel(int numFaces, string type)
{
    m_faces = numFaces;

    // Pick m_type based on the input string
    if      (m_faces > 0)            { m_type = NGON;         }
    else if (type == "Roof.flat")    { m_type = ROOF_FLAT;    }
    else if (type == "Roof.gambrel") { m_type = ROOF_GAMBREL; }
    else if (type == "Roof.cone")    { m_type = ROOF_CONE;    }
    else if (type == "Roof.gabled")  { m_type = ROOF_GABLED;  }
    else if (type == "Roof.hipped")  { m_type = ROOF_HIPPED;  }
    else if (type == "Roof.mansard") { m_type = ROOF_MANSARD; }
    else { cerr << "ERROR: Don't know what kind of mass-model this is: " << type << endl; m_type = NOTYPE; }
}

MassModel::~MassModel() { }

void MassModel::decompose(string arg, GrammarNode* op, Feature* feat, Factory* fac, Scope scope)
{
    if (arg == "sidefaces") { sidefaces(op, feat, fac, scope); }
    else if (arg == "topfaces") { topfaces(op, feat, fac, scope); }
    else {cerr << "ERROR: Composition operation of type[" << arg << "] not supported." << endl; }
}

void MassModel::sidefaces(GrammarNode* op, Feature* feat, Factory* fac, Scope scope)
{
    if (m_faces == 4 && m_type == NGON)
    {
        for (int i = 0 ; i < m_faces ; ++i)
        {
            // TODO; figure out a way to subdivide a rectangle into an ngon nicely

            // Move it along X basis
            scope = scope.translate(scope.getXBasis() * scope.getScale().x);

            // Negate the X basis
            scope = scope.setBasisComponent(0,-scope.getXBasis());

            // Swap X and Z basis
            Vector4 temp = scope.getBasisComponent(0);
            scope = scope.setBasisComponent(0, scope.getBasisComponent(2)).setBasisComponent(2, temp);
            double temp2 = scope.getScale().x;
            scope = scope.setScaleComponent(scope.getScale().z, 0).setScaleComponent(temp2, 2);

            // Evaluate
            op->evaluate(feat, *fac, scope.setScaleComponent(0.0, 2));
        }
    }
    else if (m_type == ROOF_GABLED)
    {
        double temp = scope.getScale().z;
        Scope original = scope;

        // Front facing is a roof; go to the first sidefacing by translating by X and then rotating 90 degrees to keep bases consistent
        scope = original.translate(original.getScale().x * original.getBasisComponent(0)).rotateY(-90.0, false).setScaleComponent(0.0, 2);
        op->evaluate(feat, *fac, scope.setScaleComponent(temp, 0));
        feat->getChild(feat->getNumChildren()-1)->setTriangle(true);

        scope = original.translate(original.getScale().z * original.getBasisComponent(2)).rotateY(90.0, false).setScaleComponent(0.0, 2);
        op->evaluate(feat, *fac, scope.setScaleComponent(temp, 0));
        feat->getChild(feat->getNumChildren()-1)->setTriangle(true);
    }

    feat->setActive(false);
}

void MassModel::topfaces(GrammarNode* op, Feature* feat, Factory* fac, Scope scope)
{
    if (m_type == ROOF_FLAT)
    {
        if (m_faces > 0 && m_faces != 4)
        {
            // TODO; figure out a way to subdivide a rectangle into an ngon nicely
        }
        else // Default to four sides
        {
            // Flatten Y scope
            scope = scope.setScaleComponent(0.0, 1);
            op->evaluate(feat, *fac, scope);
        }
    }
    else if (m_type == ROOF_GABLED)
    {
        Scope original = scope;

        // First slanty thing
        double len = sqrt((scope.getScale().z / 2.0) * (scope.getScale().z / 2.0) + scope.getScale().y * scope.getScale().y);
        scope = scope.rotateX(atan2(scope.getScale().z / 2.0, scope.getScale().y) * 180.0 / M_PI, false);
        scope = scope.setScaleComponent(len, 1);
        op->evaluate(feat, *fac, scope.setScaleComponent(0.0, 2));

        // Go to the other corner
        scope = original.translate(original.getScale().x * original.getBasisComponent(0));
        scope = scope.translate(original.getScale().z * original.getBasisComponent(2));
        scope = scope.setBasisComponent(0, -scope.getBasisComponent(0));
        scope = scope.setBasisComponent(2, -scope.getBasisComponent(2));

        // Second slanty thing
        scope = scope.rotateX(atan2(scope.getScale().z / 2.0, scope.getScale().y) * 180.0 / M_PI, false);
        scope = scope.setScaleComponent(len, 1);
        op->evaluate(feat, *fac, scope.setScaleComponent(0.0, 2));
    }
    else if (m_type == ROOF_CONE);
    else if (m_type == ROOF_GAMBREL);
    else if (m_type == ROOF_HIPPED);
    else if (m_type == ROOF_MANSARD);
    else { cerr << "ERROR: Topface composition called but this is not a known roof type." << endl; }
}

