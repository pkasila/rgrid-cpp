//
// Created by Pavel Kasila on 3.08.23.
//

#ifndef Z3_RESISTORGRID_H
#define Z3_RESISTORGRID_H

#include "GridPoint.h"
#include "Resistor.h"
#include "Equation.h"

class ResistorGrid {
private:
    int _width;
    int _height;
    int _depth;

    vector<vector<vector<GridPoint *>>> _points;
    vector<Resistor *> _resistors;
public:
    ResistorGrid(int width, int height, int depth);
    ~ResistorGrid();

    double calculate(const Coordinate &power, const Coordinate &ground);
};


#endif //Z3_RESISTORGRID_H
