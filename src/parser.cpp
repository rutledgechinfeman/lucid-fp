#include <fstream>
#include <iostream>
#include <string>
#include <vector>

#include "factory.h"
#include "grammar.h"
#include "parser.h"
#include "probabilitynode.h"
#include "stringutil.h"

Parser::Parser(){ }

Parser::~Parser(){ }

bool Parser::parseFile(string filename, Grammar &grammar, Factory &fac)
{
    // Open the input grammar file
    ifstream myfile(filename.c_str());
    if (!myfile.is_open() || !myfile.good())
    {
        cerr << "ERROR: Could not open grammar file: " << filename << endl;
        return false;
    }

    // Get the working directory
    string dir = filename.substr(0, filename.find_last_of("/") + 1);

    // The first line should be a symbol file (.sym), go open that and parse it
    string line;
    getline(myfile, line);
    parseSymbolFile(dir + line, fac);

    // The second line is the rule file (.rul)
    getline(myfile, line);
    parseRuleFile(dir + line, grammar);

    // Clean up
    myfile.close();
    return true;
}

bool Parser::parseSymbolFile(string filename, Factory &fac)
{
    // Open the input symbol file
    ifstream symFile(filename.c_str());
    if (!symFile.is_open() || !symFile.good())
    {
        cerr << "ERROR: Could not open symbol file: " << filename << endl;
        return false;
    }

    // Parse each symbol, line by line
    string line;
    vector<string> tokens;
    while (symFile.good())
    {
        tokens.clear();
        getline(symFile, line);

        // Ignore empty lines (for some reason we get empty lines at the end)
        if (line.size() == 0) continue;

        // Ignore invalid lines
        StringUtil::split(line, " ", tokens);
        if (tokens.size() < 2)
        {
            cerr << "WARNING: Ignoring symbol file line with less than 2 tokens: " << line << endl;
            continue;
        }

        // Nonterminal symbols
        if (tokens.at(1) == "n")
        {
            // Defensive coding; make sure we at least have the right number of tokens
            if (tokens.size() == 2) { fac.addFeatureType(tokens.at(0), false); }
            else { cerr << "WARNING: Ignoring invalid nonterminal declaration in symbol file: " << line << endl; }
        }
        else
        {
            // Defensive coding; make sure we at least have the right number of tokens
            if (tokens.size() == 4) { fac.addFeatureType(tokens.at(0), true, tokens.at(2), tokens.at(3)); }
            else { cerr << "WARNING: Ignoring invalid terminal declaration in symbol file: " << line << endl; }
        }
    }

    // Clean up
    symFile.close();
    return true;
}

bool Parser::parseRuleFile(string filename, Grammar &gram)
{
    // Open input rule file
    ifstream ruleFile(filename.c_str());
    if (!ruleFile.is_open() || !ruleFile.good())
    {
        cerr << "ERROR: Could not open rule file: " << filename << endl;
        return false;
    }

    // Parse each rule, line by line
    string original;
    string line;
    while(ruleFile.good())
    {
        getline(ruleFile, original);
        line = original; // Keep original copy for error reporting
        if (line.size() == 0 || StringUtil::trim(line).find("#") == 0) continue;

        // Peel out predecessor id
        string pred = line.substr(0, line.find(" "));
        if (pred.size() == 0) { cerr << "WARNING: Ignoring malformed rule file line: " << original << endl; continue; }

        // Remove ":" delimiter and subsequent spaces
        line = line.substr(line.find(":") + 1);
        line = StringUtil::trim(line);

        // Peel out condition string
        string cond = StringUtil::trim(line.substr(0, line.find("~")));
        if (cond.size() == 0) { cerr << "WARNING: Ignoring malformed rule file line: " << original << endl; continue; }

        // Remove fluff to get to the rule
        string rule = StringUtil::trim(line.substr(line.find("~")+1));
        if (rule.size() == 0) { cerr << "WARNING: Ignoring malformed rule file line: " << original << endl; continue; }

        gram.addRule(pred, new Rule(pred, new ProbabilityNode(rule), Condition(cond)));
    }

    // Clean up
    ruleFile.close();
    return true;
}

