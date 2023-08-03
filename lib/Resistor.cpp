//
// Created by Pavel Kasila on 3.08.23.
//

#include "Resistor.h"
#include "GridPoint.h"
#include "../include/shortcuts.h"

Resistor::Resistor(GridPoint *inflow, GridPoint *outflow) : _inflow(inflow), _outflow(outflow) {
    this->_name = SS("resistor_" << this->_inflow->get_name() << '_' << this->_outflow->get_name());
}

string Resistor::get_name() {
    return this->_name;
}

GridPoint *Resistor::get_inflow() {
    return this->_inflow;
}

GridPoint *Resistor::get_outflow() {
    return this->_outflow;
}