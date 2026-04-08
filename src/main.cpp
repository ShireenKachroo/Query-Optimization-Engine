#include <iostream>
#include "planner/Node.h"
#include "planner/QueryPlan.h"

using namespace std;

void printTree(Node* root, int level = 0) {
    if (!root) return;

    for (int i = 0; i < level; i++) cout << "  ";
    cout << root->type;

    if (!root->value.empty())
        cout << " (" << root->value << ")";

    cout << endl;

    for (auto child : root->children) {
        printTree(child, level + 1);
    }
}

int main() {

    // Dummy Query (acts like parser output)
    Query q;
    q.selectCols = {"name"};
    q.tables = {"A", "B"};
    q.conditions = {"A.id = B.id", "A.age > 20"};

    Node* plan = QueryPlan::buildPlan(q);

    cout << "Generated Query Plan:\n";
    printTree(plan);

    return 0;
}