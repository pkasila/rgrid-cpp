//
// Created by Pavel Kasila on 3.08.23.
//

#include "ResistorGrid.h"

ResistorGrid::ResistorGrid(int width, int height, int depth) {
    this->_width = width;
    this->_height = height;
    this->_depth = depth;

    if (_width < 1 || _height < 1 || _depth < 0) {
        throw runtime_error("invalid width, height, depth");
    }

    vector<vector<vector<GridPoint *>>> points(
            width + 1,
            vector<vector<GridPoint *>>(
                    height + 1,
                    vector<GridPoint *>(
                            depth + 1,
                            nullptr
                    )
            )
    );

    for (int z = 0; z < depth + 1; z++) {
        for (int y = 0; y < height + 1; y++) {
            for (int x = 0; x < width + 1; x++) {
                auto p = new GridPoint(x, y, z);
                points[x][y][z] = p;
            }
        }
    }

    this->_points = points;

    vector<Resistor *> resistors;
    resistors.reserve((width + 1) * (height + 1) * (depth + 1) * 3);

    for (int z = 0; z < depth + 1; z++) {
        for (int y = 0; y < height + 1; y++) {
            for (int x = 0; x < width + 1; x++) {
                GridPoint *p1 = this->_points[x][y][z];
                if (x < width) {
                    GridPoint *p2 = this->_points[x + 1][y][z];
                    auto *r = new Resistor(p1, p2);
                    p2->add_outflow(r);
                    p1->add_inflow(r);
                    resistors.push_back(r);
                }
                if (y < height) {
                    GridPoint *p2 = this->_points[x][y + 1][z];
                    auto *r = new Resistor(p1, p2);
                    p2->add_outflow(r);
                    p1->add_inflow(r);
                    resistors.push_back(r);
                }
                if (z < depth) {
                    GridPoint *p2 = this->_points[x][y][z + 1];
                    auto *r = new Resistor(p1, p2);
                    p2->add_outflow(r);
                    p1->add_inflow(r);
                    resistors.push_back(r);
                }
            }
        }
    }

    this->_resistors = resistors;
}

ResistorGrid::~ResistorGrid() {
    for (const auto &pi: this->_points) {
        for (const auto &pj: pi) {
            for (auto p: pj) {
                delete p;
            }
        }
    }

    for (Resistor *r : this->_resistors) {
        delete r;
    }
}

double ResistorGrid::calculate(const Coordinate &power, const Coordinate &ground) {
    vector<Equation *> eqns;
    eqns.reserve(this->_width * this->_height * this->_depth + this->_resistors.size());

    for (const auto &pi: this->_points) {
        for (const auto &pj: pi) {
            for (auto p: pj) {
                auto *e = new Equation();
                auto pc = p->get_coordinate();
                if (pc.x == power.x && pc.y == power.y && pc.z == power.z) {
                    e->add_rhs(1);
                } else if (pc.x == ground.x && pc.y == ground.y && pc.z == ground.z) {
                    continue;
                }

                for (Resistor *r: p->get_inflows()) {
                    e->add_lhs(r->get_name(), 1);
                }

                for (Resistor *r: p->get_outflows()) {
                    e->add_lhs(r->get_name(), -1);
                }

                eqns.push_back(e);
            }
        }
    }

    for (Resistor *r: this->_resistors) {
        auto *e = new Equation();
        e->add_lhs(r->get_inflow()->get_name(), 1);
        e->add_lhs(r->get_outflow()->get_name(), -1);
        e->add_lhs(r->get_name(), -1);
        eqns.push_back(e);
    }

    auto *e = new Equation();
    e->add_lhs(this->_points[ground.x][ground.y][ground.z]->get_name(), 1);
    eqns.push_back(e);

    vector<string> vlist;
    vlist.reserve(this->_width * this->_height * this->_depth + this->_resistors.size());

    for (auto pi: this->_points) {
        for (auto pj: pi) {
            for (auto p: pj) {
                vlist.push_back(p->get_name());
            }
        }
    }

    for (auto r: this->_resistors) {
        vlist.push_back(r->get_name());
    }

    vector<vector<double>> mtx(eqns.size());
    int n = mtx.size();

    for (int i = 0; i < eqns.size(); i++) {
        mtx[i] = eqns[i]->expand(vlist);
    }

    for (int i = 0; i < n; i++) {
        double maxEl = abs(mtx[i][i]);
        int maxRow = i;

        for (int k = i + 1; k < n; k++) {
            if (abs(mtx[k][i]) > maxEl) {
                maxEl = abs(mtx[k][i]);
                maxRow = k;
            }
        }

        if (maxEl == 0) {
            throw runtime_error("singular matrix");
        }

        for (int k = i; k < n + 1; k++) {
            double t = mtx[maxRow][k];
            mtx[maxRow][k] = mtx[i][k];
            mtx[i][k] = t;
        }

        for (int k = i + 1; k < n; k++) {
            double c = -mtx[k][i] / mtx[i][i];
            for (int j = i; j < n + 1; j++) {
                if (i == j) {
                    mtx[k][j] = 0;
                } else {
                    mtx[k][j] += c * mtx[i][j];
                }
            }
        }
    }

    vector<double> vals(n, 0);

    for (int i = n - 1; i > -1; i--) {
        vals[i] = mtx[i][n] / mtx[i][i];
        for (int k = n - 1; k > -1; k--) {
            mtx[k][n] -= mtx[k][i] * vals[i];
        }
    }

    double res = vals[std::find(vlist.begin(), vlist.end(), this->_points[power.x][power.y][power.z]->get_name()) -
                      vlist.begin()];

    // cleanup

    for (Equation *eqn : eqns)
        delete eqn;

    return res;
}
