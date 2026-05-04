// #include <iostream>
// #include <iomanip>
// #include "planner/Node.h"
// #include "optimizer/Optimiser.h"
// #include "cost/CostEstimator.h"

// void printTree(Node* root, int level = 0) {
//     if (!root) return;
//     for (int i = 0; i < level; i++) std::cout << "  ";
//     std::cout << "|-- " << root->type;
//     if (!root->value.empty()) std::cout << " (" << root->value << ")";
//     std::cout << std::endl;
//     for (Node* child : root->children) {
//         printTree(child, level + 1);
//     }
// }

// int main() {
//     std::cout << " SQL QUERY OPTIMIZATION ENGINE " << std::endl;

//     // BUILD UNOPTIMIZED PLAN ---
//     // Structure: LIMIT -> PROJECT -> SELECT -> JOIN -> (TABLE A, TABLE B)
//     // Note: We put Table A (1000) on left and Table B (500) on right to test Reordering
//     Node* tableA = new Node("TABLE", "A");
//     Node* tableB = new Node("TABLE", "B");

//     Node* join = new Node("JOIN");
//     join->addChild(tableA);
//     join->addChild(tableB);

//     Node* select = new Node("SELECT", "A.age > 20");
//     select->addChild(join);

//     Node* project = new Node("PROJECT", "name, age");
//     project->addChild(select);

//     Node* root = new Node("LIMIT", "10");
//     root->addChild(project);

//     std::cout << "\n[1] Initial Query Plan (Unoptimized):" << std::endl;
//     printTree(root);

//     // calc initial cost
//     double initialCost = CostEstimator::estimateCost(root);

//     // run ptimization pipeline
//     Node* optimizedRoot = Optimiser::optimise(root);

//     std::cout << "\n[2] Optimized Query Plan:" << std::endl;
//     printTree(optimizedRoot);

//     // calc final cost and report
//     double finalCost = CostEstimator::estimateCost(optimizedRoot);
//     double improvement = ((initialCost - finalCost) / initialCost) * 100;
//     std::cout << " OPTIMIZATION REPORT " << std::endl;
//     std::cout << std::fixed << std::setprecision(2);
//     std::cout << "Initial Estimated Cost   : " << initialCost << std::endl;
//     std::cout << "Optimized Estimated Cost : " << finalCost << std::endl;
//     std::cout << "Efficiency Improvement   : " << improvement << "%" << std::endl;

//     return 0;
// }




#include <iostream>
#include <iomanip>

#include "parser/parser.h"
#include "planner/QueryPlan.h"
#include "optimizer/Optimiser.h"
#include "cost/CostEstimator.h"
#include "visualisation/visualisation.h"

using namespace std;

void printTree(Node* root, int level = 0) {
    if (!root) return;

    for (int i = 0; i < level; i++) cout << "  ";
    cout << "|-- " << root->type;

    if (!root->value.empty())
        cout << " (" << root->value << ")";

    cout << endl;

    for (Node* child : root->children) {
        printTree(child, level + 1);
    }
}

void runQuery(string sql, string name) {
    cout << "\n====================================\n";
    cout << "SQL Query: " << sql << endl;

    Query q = Parser::parseSQL(sql);

    Node* root = QueryPlan::buildPlan(q);

    cout << "\nBefore Optimization:\n";
    printTree(root);

    double initialCost = CostEstimator::estimateCost(root);

    Visualization::generateDOT(root, name + "_before.dot");

    Node* optimizedRoot = Optimiser::optimise(root);

    cout << "\nAfter Optimization:\n";
    printTree(optimizedRoot);

    double finalCost = CostEstimator::estimateCost(optimizedRoot);

    Visualization::generateDOT(optimizedRoot, name + "_after.dot");

    cout << fixed << setprecision(2);
    cout << "\nOptimization Report:\n";
    cout << "Initial Cost   : " << initialCost << endl;
    cout << "Optimized Cost : " << finalCost << endl;

    if (initialCost > 0) {
        double improvement = ((initialCost - finalCost) / initialCost) * 100;
        cout << "Improvement    : " << improvement << "%\n";
    }
}

// int main() {

//     runQuery("SELECT name, age FROM A,B WHERE A.age > 20", "q1");
//     runQuery("SELECT name FROM A,B,C WHERE A.id = B.id AND B.id = C.id", "q2");
//     runQuery("SELECT age FROM B WHERE B.age > 25", "q3");
//     runQuery("SELECT name FROM A,C WHERE A.id = C.id", "q4");

//     return 0;
// }

int main() {
    string sql;
    int queryCount = 1;

    cout << "====================================\n";
    cout << "     Mini Query Optimizer\n";
    cout << "Type 'exit' to quit\n";
    cout << "====================================\n";

    while (true) {
        cout << "\nEnter SQL Query: ";
        getline(cin, sql);

        // exit condition
        if (sql == "exit") {
            cout << "\nExiting...\n";
            break;
        }

        // skip empty input
        if (sql.empty()) {
            cout << "Please enter a valid query.\n";
            continue;
        }

        string name = "q" + to_string(queryCount);

        runQuery(sql, name);

        queryCount++;
    }

    return 0;
}
