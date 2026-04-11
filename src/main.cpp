#include <iostream>
#include <iomanip>
#include "planner/Node.h"
#include "optimizer/Optimiser.h"
#include "cost/CostEstimator.h"

// Helper function to print the tree structure to the console
void printTree(Node* root, int level = 0) {
    if (!root) return;
    for (int i = 0; i < level; i++) std::cout << "  ";
    std::cout << "|-- " << root->type;
    if (!root->value.empty()) std::cout << " (" << root->value << ")";
    std::cout << std::endl;
    for (Node* child : root->children) {
        printTree(child, level + 1);
    }
}

int main() {
    std::cout << "==========================================" << std::endl;
    std::cout << "      SQL QUERY OPTIMIZATION ENGINE       " << std::endl;
    std::cout << "==========================================" << std::endl;

    // --- STEP 1: BUILD UNOPTIMIZED PLAN ---
    // Structure: LIMIT -> PROJECT -> SELECT -> JOIN -> (TABLE A, TABLE B)
    // Note: We put Table A (1000) on left and Table B (500) on right to test Reordering
    
    Node* tableA = new Node("TABLE", "A"); 
    Node* tableB = new Node("TABLE", "B");

    Node* join = new Node("JOIN");
    join->addChild(tableA);
    join->addChild(tableB);

    Node* select = new Node("SELECT", "A.age > 20");
    select->addChild(join);

    Node* project = new Node("PROJECT", "name, age");
    project->addChild(select);

    Node* root = new Node("LIMIT", "10");
    root->addChild(project);

    std::cout << "\n[1] Initial Query Plan (Unoptimized):" << std::endl;
    printTree(root);

    // --- STEP 2: CALCULATE INITIAL COST ---
    double initialCost = CostEstimator::estimateCost(root);

    // --- STEP 3: RUN OPTIMIZATION PIPELINE ---
    // This runs: Limit Pushdown -> Selection Pushdown -> Join Reordering -> Projection Pushdown
    Node* optimizedRoot = Optimiser::optimise(root);

    std::cout << "\n[2] Optimized Query Plan:" << std::endl;
    printTree(optimizedRoot);

    // --- STEP 4: CALCULATE FINAL COST & REPORT ---
    double finalCost = CostEstimator::estimateCost(optimizedRoot);
    double improvement = ((initialCost - finalCost) / initialCost) * 100;

    std::cout << "\n==========================================" << std::endl;
    std::cout << "           OPTIMIZATION REPORT            " << std::endl;
    std::cout << "==========================================" << std::endl;
    std::cout << std::fixed << std::setprecision(2);
    std::cout << "Initial Estimated Cost   : " << initialCost << std::endl;
    std::cout << "Optimized Estimated Cost : " << finalCost << std::endl;
    std::cout << "Efficiency Improvement   : " << improvement << "%" << std::endl;
    std::cout << "==========================================" << std::endl;

    return 0;
}