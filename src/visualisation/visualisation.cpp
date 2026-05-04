#include "visualisation.h"
#include <fstream>
#include <iostream>

using namespace std;

int globalNodeId = 0;

// Recursive function to create nodes + edges
int generateNode(Node* root, ofstream& file) {
    if (!root) return -1;

    int currentId = globalNodeId++;

    // Node label
    file << "node" << currentId << " [label=\""
         << root->type;

    if (!root->value.empty()) {
        file << "\\n" << root->value;
    }

    file << "\"];\n";

    // Process children
    for (Node* child : root->children) {
        int childId = generateNode(child, file);
        if (childId != -1) {
            file << "node" << currentId << " -> node" << childId << ";\n";
        }
    }

    return currentId;
}

void Visualization::generateDOT(Node* root, const string& filename) {
    ofstream file(filename);

    if (!file.is_open()) {
        cout << "Error opening file: " << filename << endl;
        return;
    }

    file << "digraph QueryPlan {\n";
    file << "node [shape=box, style=filled, color=lightblue];\n";

    globalNodeId = 0;
    generateNode(root, file);

    file << "}\n";
    file.close();
}