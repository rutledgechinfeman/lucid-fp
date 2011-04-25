#ifndef PARSER_H
#define PARSER_H

#include <map>
#include <vector>
#include <string>

using namespace std;

class Factory;
class Grammar;

class Parser
{
public:
    Parser();

    /*
     * This is the only important method of this class, it takes a
     * filename that refers to a .gmr file it parses the grammer and
     * fills in the passed hash map with grammar rules.
     * It returns a pointer to a factory instance that will generate
     * feature objects for the building.
     */
    bool parseFile(string filename, Grammar &grammar, Factory &fac);

};

#endif // PARSER_H
