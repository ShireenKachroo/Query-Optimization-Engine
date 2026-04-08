#ifndef OPTIMIZER_H
#define OPTIMIZER_H

#include "../planner/Node.h"

class Optimizer {
public:
    static Node* pushSelection(Node* root);
};

#endif