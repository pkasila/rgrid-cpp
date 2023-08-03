//
// Created by Pavel Kasila on 3.08.23.
//

#include "Equation.h"

Equation::Equation() {

}

void Equation::add_lhs(const std::string &var, int scalar) {
    lhs[var] += scalar;
}

void Equation::add_rhs(int val) {
    rhs += val;
}

vector<double> Equation::expand(const vector<std::string> &vlist) {
    vector<double> clist;

    clist.reserve(vlist.size());
    for (const string &v: vlist) {
        clist.push_back(lhs[v]);
    }
    clist.push_back(rhs);

    return clist;
}
