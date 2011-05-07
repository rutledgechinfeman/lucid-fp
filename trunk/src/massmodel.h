#ifndef MASSMODEL_H
#define MASSMODEL_H

#include <string>

class Feature;
class GrammarNode;
class Factory;
class Scope;

using std::string;

#define NOTYPE         -1
#define ROOF_GAMBREL    100
#define ROOF_CONE       101
#define ROOF_GABLED     102
#define ROOF_HIPPED     103
#define ROOF_MANSARD    104
#define NGON            200


class MassModel
{
public:
    MassModel(int numFaces = -1, string type = "");
    virtual ~MassModel();
    void decompose(string arg, GrammarNode* op, Feature* feat, Factory* fac, Scope scope);

private:
    int m_faces;
    int m_type;
};

#endif // MASSMODEL_H
