#include "QueryPlan.h"

Node* buildJoinTree(const vector<string>& tables) {
    if (tables.empty()) return nullptr;

    Node* root = new Node("TABLE", tables[0]); // first table

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

    // build join tree
    Node* root = buildJoinTree(q.tables);

    // add select cond
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

    // add node
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