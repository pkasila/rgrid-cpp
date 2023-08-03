//
// Created by Pavel Kasila on 3.08.23.
//

#ifndef Z3_GRIDPOINT_H
#define Z3_GRIDPOINT_H

#include <string>
#include "Coordinate.h"

class Resistor;

using namespace std;

class GridPoint {
private:
    Coordinate _coordinate;
    string _name;
    vector<Resistor *> _inflows;
    vector<Resistor *> _outflows;
public:
    GridPoint(int x, int y, int z);

    void add_inflow(Resistor *r);

    vector<Resistor *> get_inflows();

    void add_outflow(Resistor *r);

    vector<Resistor *> get_outflows();

    string get_name();

    Coordinate get_coordinate();
};


#endif //Z3_GRIDPOINT_H
