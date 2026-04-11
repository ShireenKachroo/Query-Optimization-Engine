#ifndef OPTIMISER_H
#define OPTIMISER_H

#include "../planner/Node.h"

class Optimiser {
public:
    static Node* pushSelection(Node* root);
    static Node* pushProjection(Node* root);
    static Node* pushLimit(Node* root);       // New!
    static Node* reorderJoins(Node* root);    // New!
    static Node* optimise(Node* root);
};

#endif