#ifndef REFEREE_H
#define REFEREE_H
#include <iostream>
#include <string>
#include "parser.h"
#include "grammar.h"
#include "factory.h"
#include "contractor.h"
#include <string>

using namespace std;


class Referee
{
public:
    Referee(string file = "");
    virtual ~Referee();
    Feature* restart(string file);
    Feature* restart();
    Feature* refresh();
    string getString(){ return m_file; }

private:
    void init();
    void cleanup();
    string m_file;
    Parser* m_parser;
    Grammar* m_grammar;
    Factory* m_fac;
    Contractor* m_contractor;
};

#endif // REFEREE_H
