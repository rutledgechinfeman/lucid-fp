#include "parser.h"

Parser::Parser()
{
}

Factory* Parser::parseFile(string filename, std::map<string, std::vector<Rule> > &ruleMap);
