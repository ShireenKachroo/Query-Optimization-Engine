#include "QueryPlan.h"

// Helper: create JOIN tree from tables
Node* buildJoinTree(const vector<string>& tables) {
    if (tables.empty()) return nullptr;

    // Start with first table
    Node* root = new Node("TABLE", tables[0]);

    for (int i = 1; i < tables.size(); i++) {
        Node* newTable = new Node("TABLE", tables[i]);
        Node* joinNode = new Node("JOIN","");

        joinNode->addChild(root);
        joinNode->addChild(newTable);

        root = joinNode;
    }

    return root;
}

Node* QueryPlan::buildPlan(const Query& q) {

    // Step 1: Build JOIN tree from tables
    Node* root = buildJoinTree(q.tables);

    // Step 2: Add SELECT node (if conditions exist)
    if (!q.conditions.empty()) {
        string combinedCondition = "";
        for (int i = 0; i < q.conditions.size(); i++) {
            combinedCondition += q.conditions[i];
            if (i != q.conditions.size() - 1)
                combinedCondition += " AND ";
        }

        Node* selectNode = new Node("SELECT", combinedCondition);
        selectNode->addChild(root);
        root = selectNode;
    }

    // Step 3: Add PROJECT node
    if (!q.selectCols.empty()) {
        string cols = "";
        for (int i = 0; i < q.selectCols.size(); i++) {
            cols += q.selectCols[i];
            if (i != q.selectCols.size() - 1)
                cols += ", ";
        }

        Node* projectNode = new Node("PROJECT", cols);
        projectNode->addChild(root);
        root = projectNode;
    }

    return root;
}