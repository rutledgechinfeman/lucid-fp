#include "referee.h"

Referee::Referee(string file)
{
    m_file = file;

    m_parser = NULL;
    m_grammar = NULL;
    m_fac = NULL;
    m_contractor = NULL;
}

Referee::~Referee()
{
    cleanup();
}

Feature* Referee::restart()
{
    cleanup();
    init();

    m_parser->parseFile(m_file, *m_grammar, *m_fac);
    return m_contractor->build(m_grammar, m_fac);
}

Feature* Referee::restart(string file)
{
    cleanup();
    init();
    m_file = file;
    m_parser->parseFile(file, *m_grammar, *m_fac);
    return m_contractor->build(m_grammar, m_fac);
}

Feature* Referee::refresh()
{
    if (m_parser == NULL && m_grammar == NULL && m_fac == NULL && m_contractor == NULL)
    {
        cerr << "ERROR: Referee must call restart at least once before refresh." << endl;
    }

    return m_contractor->build(m_grammar, m_fac);
}

void Referee::init()
{
    m_parser = new Parser();
    m_grammar = new Grammar();
    m_fac = new Factory();
    m_contractor = new Contractor();
}

void Referee::cleanup()
{
    delete m_contractor; // delete contractor first, it contains/uses a parser and a grammar
    delete m_parser;
    delete m_grammar;
    delete m_fac;
}
