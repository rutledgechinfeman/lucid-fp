#include "referee.h"

Referee::Referee(){}

Referee::~Referee(){
    delete m_parser;
    delete m_grammar;
    delete m_fac;
    delete m_contractor;
}


Referee::Referee(string file)
{
    m_file = file;
    m_parser = new Parser();
    m_grammar = new Grammar();
    m_fac = new Factory();
    m_contractor = new Contractor();
}

bool Referee::blowWhistle(){
    m_parser->parseFile(m_file, *m_grammar, *m_fac);
    m_contractor->build(*m_grammar, *m_fac);

    return true;
}

