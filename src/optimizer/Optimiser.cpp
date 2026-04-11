#include "Optimiser.h"

// -------------------- Selection Pushdown --------------------
Node* Optimiser::pushSelection(Node* root) {
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
Node* Optimiser::pushProjection(Node* root) {
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


Node* Optimiser::pushLimit(Node* root) {
    if (!root || root->children.empty()) return root;

    if (root->type == "LIMIT") {
        Node* child = root->children[0];
        if (child->type == "JOIN") {
            // Push the same limit to both children of the join
            Node* leftLimit = new Node("LIMIT", root->value);
            leftLimit->addChild(child->children[0]);

            Node* rightLimit = new Node("LIMIT", root->value);
            rightLimit->addChild(child->children[1]);

            child->children[0] = leftLimit;
            child->children[1] = rightLimit;
        }
    }

    for (auto &c : root->children) c = pushLimit(c);
    return root;
}

#include "../cost/CostEstimator.h" // Needed for getOutputSize

Node* Optimiser::reorderJoins(Node* root) {
    if (!root || root->children.empty()) return root;

    // Process bottom-up
    for (auto &c : root->children) c = reorderJoins(c);

    if (root->type == "JOIN" && root->children.size() == 2) {
        Node* left = root->children[0];
        Node* right = root->children[1];

        // We want the "smaller" table on the left (Heuristic for many join algorithms)
        // Note: This requires getOutputSize to be accessible or moved to a utility
        if (CostEstimator::estimateCost(left) > CostEstimator::estimateCost(right)) {
            // Swap children
            root->children[0] = right;
            root->children[1] = left;
        }
    }
    return root;
}

Node* Optimiser::optimise(Node* root) {
    if (!root) return nullptr;

    // 1. Move limits down to stop processing early
    root = pushLimit(root);

    // 2. Prune rows as early as possible
    root = pushSelection(root);

    // 3. Now that rows are pruned, check if we should swap join order
    root = reorderJoins(root);

    // 4. Finally, prune columns
    root = pushProjection(root);

    return root;
}