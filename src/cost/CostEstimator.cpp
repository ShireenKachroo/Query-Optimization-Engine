#include "CostEstimator.h"
#include <unordered_map>

unordered_map<string, int> tableSizes = {{"A", 1000}, {"B", 500}, {"C", 200}};

double getOutputSize(Node* root) {
    if (!root) return 0;
    if (root->type == "TABLE") return tableSizes[root->value];

    if (root->type == "SELECT") return getOutputSize(root->children[0]) * 0.2;

    if (root->type == "PROJECT") return getOutputSize(root->children[0]);

    if (root->type == "JOIN") {
        return getOutputSize(root->children[0]) * getOutputSize(root->children[1]) * 0.01;
    }
    return 0;
}

double getWidthFactor(Node* root) {
    if (root->type == "PROJECT") {
        int cols = 1;
        for (char c : root->value) if (c == ',') cols++;
        return cols * 0.1;
    }
    return 1.0;
}

double CostEstimator::estimateCost(Node* root) {
    if (!root) return 0;

    double childrenCost = 0;
    for (Node* child : root->children) {
        childrenCost += estimateCost(child);
    }

    double currentCost = 0;
    double width = getWidthFactor(root);

    if (root->type == "JOIN") {
        currentCost = (getOutputSize(root->children[0]) * getOutputSize(root->children[1])) * width;
    }
    else if (root->type == "SELECT") {
        currentCost = getOutputSize(root->children[0]);
    }

    return childrenCost + currentCost;
}