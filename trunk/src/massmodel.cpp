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
    if (m_faces < 3)
    {
        cerr << "ERROR: Cannot decompose something with less than 3 faces. Has: " << m_faces << endl;
        return;
    }

    // Flatten side faces
    scope = scope.setScaleComponent(0.0, 2);

    for (int i = 0 ; i < m_faces ; ++i)
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
        op->evaluate(feat, *fac, scope);
    }

    feat->setActive(false);
}

void MassModel::topfaces(GrammarNode* op, Feature* feat, Factory* fac, Scope scope)
{
    if (m_type == ROOF_FLAT)
    {
        scope.printSelf();
        // Flatten Y scope
        scope = scope.setScaleComponent(0.0, 1);
        op->evaluate(feat, *fac, scope);
    }
    else if (m_type == ROOF_GAMBREL);
    else if (m_type == ROOF_CONE);
    else if (m_type == ROOF_GABLED);
    else if (m_type == ROOF_HIPPED);
    else if (m_type == ROOF_MANSARD);
    else { cerr << "ERROR: Topface composition called but this is not a known roof type." << endl; }
}

