#include "Optimiser.h"

Node* Optimizer::pushSelection(Node* root) {
    if (!root || root->children.empty()) return root;

    // 1. Recursive call to process bottom-up
    for (auto& child : root->children) {
        child = pushSelection(child);
    }

    // 2. If we find a SELECT sitting on top of a JOIN
    if (root->type == "SELECT" && root->children[0]->type == "JOIN") {
        Node* joinNode = root->children[0];
        Node* leftChild = joinNode->children[0];
        Node* rightChild = joinNode->children[1];
        string condition = root->value;

        // Check if condition belongs to Left Table (e.g., "A.age")
        if (leftChild->type == "TABLE" && condition.find(leftChild->value + ".") != string::npos) {
            Node* newSelect = new Node("SELECT", condition);
            newSelect->addChild(leftChild);
            joinNode->children[0] = newSelect; // Push down to left
            return joinNode; // Return join as new root (SELECT is now below it)
        }
        
        // Check if condition belongs to Right Table
        if (rightChild->type == "TABLE" && condition.find(rightChild->value + ".") != string::npos) {
            Node* newSelect = new Node("SELECT", condition);
            newSelect->addChild(rightChild);
            joinNode->children[1] = newSelect; // Push down to right
            return joinNode;
        }
    }

    return root;
}