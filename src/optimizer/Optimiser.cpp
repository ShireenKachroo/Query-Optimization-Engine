#include "Optimiser.h"
#include "../cost/CostEstimator.h"

// SELECTION PUSHDOWN
Node* Optimiser::pushSelection(Node* root) {
    if (!root || root->children.empty()) return root;

    // bottom up
    for (auto& child : root->children) {
        child = pushSelection(child);
    }

    // select over join cond
    if (root->type == "SELECT" && root->children[0]->type == "JOIN") {
        Node* joinNode = root->children[0];
        Node* leftChild = joinNode->children[0];
        Node* rightChild = joinNode->children[1];
        string condition = root->value;

        // Push to Left Table if condition matches
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

// PROJECTION PUSHDOWN
Node* Optimiser::pushProjection(Node* root) {
    if (!root || root->children.empty()) return root;

    if (root->type == "PROJECT") {
        Node* child = root->children[0];

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

// LIMIT PUSHDOWN
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

// JOIN REORDERING
Node* Optimiser::reorderJoins(Node* root) {
    if (!root || root->children.empty()) return root;

    // Process bottom-up
    for (auto &c : root->children) c = reorderJoins(c);

    if (root->type == "JOIN" && root->children.size() == 2) {
        Node* left = root->children[0];
        Node* right = root->children[1];
        if (CostEstimator::estimateCost(left) > CostEstimator::estimateCost(right)) {
            root->children[0] = right;
            root->children[1] = left;
        }
    }
    return root;
}

// flow: Limit Pushdown -> Selection Pushdown -> Join Reordering -> Projection Pushdown
Node* Optimiser::optimise(Node* root) {
    if (!root) return nullptr;
    root = pushLimit(root);

    root = pushSelection(root);

    root = reorderJoins(root);

    root = pushProjection(root);

    return root;
}