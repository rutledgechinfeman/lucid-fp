#ifndef MASSMODEL_H
#define MASSMODEL_H

#include <string>

class Feature;
class GrammarNode;
class Factory;
class Scope;

using std::string;

#define NOTYPE         -1
#define ROOF_FLAT       100
#define ROOF_GAMBREL    101
#define ROOF_CONE       102
#define ROOF_GABLED     103
#define ROOF_HIPPED     104
#define ROOF_MANSARD    105
#define NGON            200


class MassModel
{
public:
    /// CTOR
    MassModel(int numFaces = -1, string type = "");

    /// DTOR
    virtual ~MassModel();

    /// Decompose the incoming feature into its arg components, and perform op on them
    void decompose(string arg, GrammarNode* op, Feature* feat, Factory* fac, Scope scope);

private:
    /// Helper
    void sidefaces(GrammarNode* op, Feature* feat, Factory* fac, Scope scope);

    /// Helper
    void topfaces(GrammarNode* op, Feature* feat, Factory* fac, Scope scope);

    /// Number of faces in an NGON (mutually exclusive with roofs)
    int m_faces;

    /// Roof type (mutually exclusive with ngons)
    int m_type;
};

#endif // MASSMODEL_H
