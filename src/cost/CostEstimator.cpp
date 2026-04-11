#include "CostEstimator.h"
#include <unordered_map>

unordered_map<string, int> tableSizes = {{"A", 1000}, {"B", 500}, {"C", 200}};

// Estimates how many rows come OUT of a node
double getOutputSize(Node* root) {
    if (!root) return 0;
    if (root->type == "TABLE") return tableSizes[root->value];
    
    // Selection reduces data (Heuristic: 20% selectivity)
    if (root->type == "SELECT") return getOutputSize(root->children[0]) * 0.2;
    
    if (root->type == "PROJECT") return getOutputSize(root->children[0]);

    if (root->type == "JOIN") {
        // Simple Join Size: (Left * Right) * 0.01
        return getOutputSize(root->children[0]) * getOutputSize(root->children[1]) * 0.01;
    }
    return 0;
}

double CostEstimator::estimateCost(Node* root) {
    if (!root) return 0;

    double childrenCost = 0;
    for (Node* child : root->children) {
        childrenCost += estimateCost(child);
    }

    double currentOperationCost = 0;
    if (root->type == "JOIN") {
        // Join cost depends on the size of the inputs
        currentOperationCost = getOutputSize(root->children[0]) * getOutputSize(root->children[1]);
    } else if (root->type == "SELECT") {
        currentOperationCost = getOutputSize(root->children[0]); // Cost to scan rows
    }

    return childrenCost + currentOperationCost;
}