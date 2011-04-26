#include "parser.h"
#include "factory.h"
#include "grammar.h"

#include <string>
#include <fstream>
#include <iostream>
#include <vector>



Parser::Parser(){ }

Parser::~Parser(){ }

bool Parser::parseFile(string filename, Grammar &grammar, Factory &fac)
{

    cout << filename << endl;

    string line;
    ifstream myfile(filename.c_str());

    if (!myfile.is_open() || !myfile.good()) {
        cout << "grammar file fail" << endl;
        return false;
    }

    string dir = filename.substr(0, filename.find_last_of("/") + 1);

    getline(myfile, line);
    parseSymbolFile(dir + line, fac);
    getline(myfile, line);
    parseRuleFile(dir + line, grammar);


    myfile.close();

    return true;
}




bool Parser::parseSymbolFile(string filename, Factory &fac) {

    ifstream symFile(filename.c_str());
    if (!symFile.is_open() || !symFile.good()) {
        cout << "symbol file fail" << endl;
        return false;
    }

    string line;
    vector<string> tokens;
    while(symFile.good()) {
        tokens.clear();
        getline(symFile, line);

        if (line.size() == 0) continue;

        split(line, " ", tokens);

        if (tokens.size() < 2) {
            cout << line << endl;
            cout << "symbol file line must have at least 2 tokens" << endl;
            continue;
        }

        if (tokens.at(1) == "n") {
            if (tokens.size() == 2) {
                fac.addFeatureType(tokens.at(0), false);
            }
            else {
                cout << "non terminal line in symbol file has wrong number of things. Ignoring." << endl;
            }
        }
        else {
            if (tokens.size() == 4) {
                fac.addFeatureType(tokens.at(0), true, tokens.at(2), tokens.at(3));
            }
            else {
                cout << "terminal line in symbol file has the wrong number of things. Ignoring." << endl;
            }
        }
    }

    symFile.close();
    return true;
}

bool Parser::parseRuleFile(string filename, Grammar &gram) {

    ifstream ruleFile(filename.c_str());
    if (!ruleFile.is_open() || !ruleFile.good()) {
        cout << "symbol file fail" << endl;
        return false;
    }

    string line;
    while(ruleFile.good()) {
        getline(ruleFile, line);
        if (line.size() == 0) continue;

        // Peel out predecessor id
        string pred = line.substr(0, line.find(" "));

        // Remove ":" delimiter and subsequent spaces
        line = line.substr(line.find(":") + 1);
        line = trim(line);

        // Peel out condition string
        string cond = line.substr(0, line.find("~"));

        // Remove fluff to get to the rule
        line = line.substr(line.find("~")+1);

        Rule(pred, GrammarNode(trim(line)), Condition(trim(cond))); // TODO
    }

    ruleFile.close();
    return true;
}

void Parser::split(string toSplit, char* on, vector<string> &result)
{
    int index;
    while ((index = toSplit.find(on)) != -1)
    {
        result.push_back(toSplit.substr(0, index));

        toSplit = toSplit.substr(index+1);
    }

    result.push_back(toSplit);
}

string Parser::trim(string in)
{
    while(in.find(" ") == 0)
    {
        in = in.substr(1);
    }

    while(in.find_last_of(" ") == in.size()-1 && in.size() != 0)
    {
        in = in.substr(0,in.size()-1);
    }

    return in;
}
