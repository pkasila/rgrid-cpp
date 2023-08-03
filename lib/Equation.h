//
// Created by Pavel Kasila on 3.08.23.
//

#ifndef Z3_EQUATION_H
#define Z3_EQUATION_H

#include <string>
#include <map>

using namespace std;

class Equation {
private:
    map<string, int> lhs;
    int rhs = 0;
public:
    Equation();

    void add_lhs(const string &var, int scalar);

    void add_rhs(int val);

    vector<double> expand(const vector<string> &vlist);
};


#endif //Z3_EQUATION_H
