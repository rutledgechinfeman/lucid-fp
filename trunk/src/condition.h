#ifndef CONDITION_H
#define CONDITION_H

#include <string>
#include "feature.h"

#define UNKNOWN -1

//Condition type
#define EMPTY 100
#define OCCLUDED 101
#define VISIBLE 102

//Condition parameter
#define ALL 200
#define NOPARENT 201
#define STREET 202

//righthand side of condish
#define NONE 300
#define PART 301
#define FULL 302

//target of condish
#define SHAPE 400
#define SCOPE 401

/*
 * Format is like this for an empty condition:
 * NULL
 *
 * And for a regular one:
 * Target.type(parameter) = rhs
 */

using std::string;

class Condition
{
public:
    Condition(string line = "NULL");
    virtual ~Condition();

    virtual bool evaluate(const Feature &f);


private:

    int m_type;
    int m_rhs;
    int m_param;
    int m_target;

};

#endif // CONDITION_H
