#ifndef PARSER_H
#define PARSER_H

#include "../planner/QueryPlan.h"
#include <string>

class Parser {
public:
    static Query parseSQL(const std::string& sql);
};

#endif