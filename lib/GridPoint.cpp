//
// Created by Pavel Kasila on 3.08.23.
//

#include "GridPoint.h"
#include "Resistor.h"
#include "../include/shortcuts.h"

GridPoint::GridPoint(int x, int y, int z) : _coordinate({x, y, z}) {
    this->_name = SS("GridPoint_" << _coordinate.x << '_' << _coordinate.y << '_' << _coordinate.z);
}

void GridPoint::add_inflow(Resistor *r) {
    this->_inflows.push_back(r);
}

vector<Resistor *> GridPoint::get_inflows() {
    return this->_inflows;
}

void GridPoint::add_outflow(Resistor *r) {
    this->_outflows.push_back(r);
}

vector<Resistor *> GridPoint::get_outflows() {
    return this->_outflows;
}

string GridPoint::get_name() {
    return this->_name;
}

Coordinate GridPoint::get_coordinate() {
    return this->_coordinate;
}
