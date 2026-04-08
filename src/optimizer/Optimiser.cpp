#include "Optimiser.h"
#include <string>

using namespace std;

// Check if condition belongs to a table
bool belongsToTable(string condition, string table) {
    return condition.find(table + ".") != string::npos;
}

Node* Optimizer::pushSelection(Node* root) {
    if (!root) return nullptr;

    // Recursively optimize children first
    for (auto &child : root->children) {
        child = pushSelection(child);
    }

    // Apply pushdown
    if (root->type == "SELECT") {
        Node* child = root->children[0];

        if (child->type == "JOIN") {
            string condition = root->value;

            Node* left = child->children[0];
            Node* right = child->children[1];

            // Check where condition belongs
            if (belongsToTable(condition, left->value)) {
                Node* newSelect = new Node("SELECT", condition);
                newSelect->addChild(left);
                child->children[0] = newSelect;
                return child;
            }
            else if (belongsToTable(condition, right->value)) {
                Node* newSelect = new Node("SELECT", condition);
                newSelect->addChild(right);
                child->children[1] = newSelect;
                return child;
            }
        }
    }

    return root;
}