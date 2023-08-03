//
// Created by Pavel Kasila on 3.08.23.
//

#ifndef Z3_RESISTOR_H
#define Z3_RESISTOR_H

#include <string>

using namespace std;

class GridPoint;

class Resistor {
private:
    string _name;
    GridPoint *_inflow;
    GridPoint *_outflow;
public:
    Resistor(GridPoint *inflow, GridPoint *outflow);

    GridPoint *get_inflow();

    GridPoint *get_outflow();

    string get_name();
};

#endif //Z3_RESISTOR_H
