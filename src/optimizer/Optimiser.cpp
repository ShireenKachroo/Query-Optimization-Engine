#include "Optimiser.h"

// -------------------- Selection Pushdown --------------------
Node* Optimizer::pushSelection(Node* root) {
    if (!root || root->children.empty()) return root;

    // 1. Bottom-up recursion
    for (auto& child : root->children) {
        child = pushSelection(child);
    }

    // 2. Optimization: SELECT over JOIN
    if (root->type == "SELECT" && root->children[0]->type == "JOIN") {
        Node* joinNode = root->children[0];
        Node* leftChild = joinNode->children[0];
        Node* rightChild = joinNode->children[1];
        string condition = root->value;

        // Push to Left Table if condition matches (e.g., "A.id")
        if (leftChild->type == "TABLE" && condition.find(leftChild->value + ".") != string::npos) {
            Node* newSelect = new Node("SELECT", condition);
            newSelect->addChild(leftChild);
            joinNode->children[0] = newSelect; 
            return joinNode; 
        }
        
        // Push to Right Table
        if (rightChild->type == "TABLE" && condition.find(rightChild->value + ".") != string::npos) {
            Node* newSelect = new Node("SELECT", condition);
            newSelect->addChild(rightChild);
            joinNode->children[1] = newSelect;
            return joinNode;
        }
    }

    return root;
}

// -------------------- Projection Pushdown --------------------
Node* Optimizer::pushProjection(Node* root) {
    if (!root || root->children.empty()) return root;

    if (root->type == "PROJECT") {
        Node* child = root->children[0];

        // Push mini-projections below JOIN
        if (child->type == "JOIN") {
            Node* left = child->children[0];
            Node* right = child->children[1];

            // Only push if children aren't already projected
            if (left->type == "TABLE") {
                Node* leftProj = new Node("PROJECT", "sub_cols");
                leftProj->addChild(left);
                child->children[0] = leftProj;
            }
            
            if (right->type == "TABLE") {
                Node* rightProj = new Node("PROJECT", "sub_cols");
                rightProj->addChild(right);
                child->children[1] = rightProj;
            }
        }
    }

    for (auto &c : root->children) {
        c = pushProjection(c);
    }
    
    return root;
}

// -------------------- Main Entry Point --------------------
Node* Optimizer::optimise(Node* root) {
    if (!root) return nullptr;
    
    // Sequence matters: Selection first to reduce rows, then Projection to reduce columns
    root = pushSelection(root);
    root = pushProjection(root);
    
    return root;
}