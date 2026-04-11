#include <iostream>
#include "planner/Node.h"
#include "optimizer/Optimiser.h"
#include "cost/CostEstimator.h"

int main() {
    // Manually building: PROJECT -> SELECT (A.age > 20) -> JOIN -> (A, B)
    Node* tableA = new Node("TABLE", "A");
    Node* tableB = new Node("TABLE", "B");
    
    Node* join = new Node("JOIN");
    join->addChild(tableA);
    join->addChild(tableB);

    Node* select = new Node("SELECT", "A.age > 20");
    select->addChild(join);

    Node* root = new Node("PROJECT", "name");
    root->addChild(select);

    double oldCost = CostEstimator::estimateCost(root);
    
    // Perform Optimization
    Node* optimizedRoot = Optimizer::pushSelection(root);
    double newCost = CostEstimator::estimateCost(optimizedRoot);

    std::cout << "--- Query Optimization Report ---" << std::endl;
    std::cout << "Initial Cost: " << oldCost << std::endl;
    std::cout << "Optimized Cost: " << newCost << std::endl;
    
    if (oldCost > 0) {
        double improvement = ((oldCost - newCost) / oldCost) * 100;
        std::cout << "Improvement: " << improvement << "%" << std::endl;
    }

    return 0;
}