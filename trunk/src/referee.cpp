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
    delete m_parser;
    delete m_grammar;
    delete m_fac;
    delete m_contractor;
}

bool Referee::blowWhistle()
{
    m_parser->parseFile(m_file, *m_grammar, *m_fac);
    m_contractor->build(*m_grammar, *m_fac)->draw();
    Feature f = Feature("window", "plane", true, Scope(Vector4(-1.0, 1.0, -10.0, 1.0), Vector4(2.0, 1.0, 1.0, 1.0),
                                                       Vector4(1.0, 0.0, 0.0, 0.0),
                                                       Vector4(0.5, 0.5, 0.0, 0.0), Vector4(0.0, 0.0, 1.0, 0.0)), NULL);
    f.draw();

    return true;
}

