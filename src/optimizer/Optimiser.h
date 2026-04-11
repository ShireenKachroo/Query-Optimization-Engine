#ifndef OPTIMISER_H
#define OPTIMISER_H

#include "../planner/Node.h"

class Optimiser {
public:
    static Node* pushSelection(Node* root);
    static Node* pushProjection(Node* root);
    static Node* pushLimit(Node* root);
    static Node* reorderJoins(Node* root);
    static Node* optimise(Node* root);
};

#endif