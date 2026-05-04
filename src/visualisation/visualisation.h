#ifndef VISUALIZATION_H
#define VISUALIZATION_H

#include "../planner/Node.h"
#include <string>

class Visualization {
public:
    static void generateDOT(Node* root, const std::string& filename);
};

#endif