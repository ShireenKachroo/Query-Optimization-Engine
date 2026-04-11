#ifndef QUERYPLAN_H
#define QUERYPLAN_H

#include "Node.h"
#include <vector>
#include <string>

using namespace std;

struct Query {
    vector<string> selectCols;
    vector<string> tables;
    vector<string> conditions;
};

class QueryPlan {
public:
    static Node* buildPlan(const Query& q);
};

#endif