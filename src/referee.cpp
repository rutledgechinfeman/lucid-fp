#include "referee.h"

Referee::Referee(string file)
{
    m_file = file;
    m_parser = new Parser();
    m_grammar = new Grammar();
    m_fac = new Factory();
    m_contractor = new Contractor();
}

Referee::~Referee()
{
    delete m_contractor; // delete contractor first, it contains/uses a parser and a grammar
    delete m_parser;
    delete m_grammar;
    delete m_fac;
}

Feature* Referee::blowWhistle()
{
    m_parser->parseFile(m_file, *m_grammar, *m_fac);
    return m_contractor->build(*m_grammar, *m_fac);
}

