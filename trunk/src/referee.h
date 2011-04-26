#ifndef REFEREE_H
#define REFEREE_H
#include <iostream>
#include <string>
#include "parser.h"
#include "grammar.h"
#include "factory.h"
#include "contractor.h"

using namespace std;


class Referee
{
public:
    Referee();
    Referee(string file);
    ~Referee();
    bool blowWhistle();
private:
    string m_file;
    Parser* m_parser;
    Grammar* m_grammar;
    Factory* m_fac;
    Contractor* m_contractor;
};

#endif // REFEREE_H
