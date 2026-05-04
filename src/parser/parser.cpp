#include "parser.h"
#include <sstream>
#include <algorithm>
#include <cctype>
#include <iostream>

using namespace std;

// helper: split string by delimiter
vector<string> split(const string& s, char delimiter) {
    vector<string> tokens;
    string token;
    stringstream ss(s);

    while (getline(ss, token, delimiter)) {
        // remove leading & trailing spaces
        token.erase(0, token.find_first_not_of(" \t\n\r"));
        token.erase(token.find_last_not_of(" \t\n\r") + 1);
        tokens.push_back(token);
    }
    return tokens;
}

// helper: convert string to uppercase
string toUpper(string s) {
    transform(s.begin(), s.end(), s.begin(), ::toupper);
    return s;
}

Query Parser::parseSQL(const string& sql) {
    Query q;

    string upper = toUpper(sql);

    int selectPos = upper.find("SELECT");
    int fromPos = upper.find("FROM");
    int wherePos = upper.find("WHERE");

    if (selectPos == string::npos || fromPos == string::npos) {
        cout << "Invalid SQL Query!" << endl;
        return q;
    }

    // ---------------- SELECT ----------------
    string selectPart = sql.substr(selectPos + 6, fromPos - (selectPos + 6));
    q.selectCols = split(selectPart, ',');

    // ---------------- FROM ----------------
    if (wherePos == string::npos) {
        string fromPart = sql.substr(fromPos + 4);
        q.tables = split(fromPart, ',');
    } else {
        string fromPart = sql.substr(fromPos + 4, wherePos - (fromPos + 4));
        q.tables = split(fromPart, ',');

        // ---------------- WHERE ----------------
        string wherePart = sql.substr(wherePos + 5);

        // split on AND properly
        string temp = wherePart;
        string upperWhere = toUpper(wherePart);

        size_t pos = 0;
        while ((pos = upperWhere.find("AND")) != string::npos) {
            string cond = temp.substr(0, pos);
            cond.erase(0, cond.find_first_not_of(" \t\n\r"));
            cond.erase(cond.find_last_not_of(" \t\n\r") + 1);

            q.conditions.push_back(cond);

            temp = temp.substr(pos + 3);
            upperWhere = toUpper(temp);
        }

        // last condition
        temp.erase(0, temp.find_first_not_of(" \t\n\r"));
        temp.erase(temp.find_last_not_of(" \t\n\r") + 1);
        if (!temp.empty())
            q.conditions.push_back(temp);
    }

    return q;
}