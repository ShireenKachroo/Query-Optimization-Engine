#define NODE_H
#include<iostream>
#include<vector>
#include<string>

using namespace std;

class Node{
	public:
		string type;
		string value;
		vector<Node*> children;
		Node(string t, string v){
			type = t;
			value = v;
		}

		void addChild(Node* child){
			children.push_back(child);
		}
};
