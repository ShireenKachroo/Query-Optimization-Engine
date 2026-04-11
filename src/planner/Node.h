#ifndef NODE_H
#define NODE_H

#include <vector>
#include <string>

using namespace std;

class Node {
public:
    string type;  // ex:  "SELECT", "PROJECT", "JOIN", "TABLE"
    string value; // like Condition, Column names, or Table name
    vector<Node*> children;

    Node(string t, string v = "") : type(t), value(v) {}

    void addChild(Node* child) {
        children.push_back(child);
    }
};

#endif