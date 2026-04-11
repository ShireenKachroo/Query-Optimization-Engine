#ifndef COST_ESTIMATOR_H
#define COST_ESTIMATOR_H

#include "../planner/Node.h"
#include <unordered_map>

using namespace std;

class CostEstimator {
public:
    static double estimateCost(Node* root);
    static unordered_map<string, int> tableSize;
};

#endif