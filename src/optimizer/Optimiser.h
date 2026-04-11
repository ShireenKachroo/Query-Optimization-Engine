#ifndef OPTIMISER_H
#define OPTIMISER_H

#include "../planner/Node.h"

class Optimizer {
public:
    static Node* pushSelection(Node* root);
    static Node* pushProjection(Node* root);
    static Node* optimise(Node* root); // Use this in main.cpp
};

#endif