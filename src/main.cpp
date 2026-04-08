#include <iostream>
#include "planner/Node.h"
#include<bits/stdc++.h>


using namespace std;

// printing tree
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

    // Create TABLE nodes
    Node* A = new Node("TABLE", "A");
    Node* B = new Node("TABLE", "B");

    // Create JOIN
    Node* join = new Node("JOIN","");
    join->addChild(A);
    join->addChild(B);

    // Create SELECT (filter)
    Node* select = new Node("SELECT", "A.age > 20");
    select->addChild(join);

    // Create PROJECT
    Node* project = new Node("PROJECT", "name");
    project->addChild(select);

    // Print tree
    cout << "Query Plan Tree:\n";
    printTree(project);

    return 0;
}