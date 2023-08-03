#include <iostream>
#include <fstream>
#include <iomanip>
#include "lib/ResistorGrid.h"
#include "clipp.h"

using namespace std;
using namespace clipp;

enum class mode {
    onePair, fullGrid, help
};

void runOnePair(int width, int height, int depth, const Coordinate &power, const Coordinate &ground) {
    ResistorGrid grid(width, height, depth);

    double r = grid.calculate(power, ground);

    cout << "R(" << power.x << ',' << power.y << ',' << power.z << ';' << ground.x << ',' << ground.y << ',' << ground.z
         << ") = " << std::fixed << std::setprecision(5) << r << " Ohm" << endl;
}

void runFullGrid(int width, int height, int depth, const string &output, bool progress) {
    ofstream out(output);
    out << "size,i j a,k m b, R,distance" << endl;

    ResistorGrid grid(width, height, depth);

    if (progress)
        cout << "initialized grid" << endl;

    long long t1 = duration_cast<chrono::milliseconds>(chrono::system_clock::now().time_since_epoch()).count();

    long long counter = 0;
    long long total = (width + 1) * (width + 1) * (height + 1) * (height + 1) * (depth + 1) * (depth + 1);

    cout << std::fixed << std::setprecision(5);
    out << std::fixed << std::setprecision(5);

    for (int i = 0; i < width + 1; i++) {
        for (int j = 0; j < height + 1; j++) {
            for (int a = 0; a < depth + 1; a++) {
                for (int k = 0; k < width + 1; k++) {
                    for (int m = 0; m < height + 1; m++) {
                        for (int b = 0; b < depth + 1; b++) {
                            if (i == k && j == m && a == b) {
                                continue;
                            }

                            double r = grid.calculate({i, j, a}, {k, m, b});

                            if (progress)
                                cout << '\r' << i << ' ' << j << ' ' << a << ' ' << k << ' ' << m << ' ' << b
                                     << " complete R = " << r << " Ohm";

                            int dist = abs(i - k) + abs(j - m) + abs(a - b);
                            out << width << 'x' << height << 'x' << depth << ',' << i << ' ' << j << ' ' << a << ','
                                << k << ' ' << m << ' ' << b << ',' << r << ',' << dist << endl;

                            counter++;

                            long long t = duration_cast<chrono::milliseconds>(
                                    chrono::system_clock::now().time_since_epoch()).count();
                            long long dur = (t - t1) / counter;

                            long long left = (total - counter) * dur / 1000;

                            cout << " (time left: " << left << "s)";

                            if (progress)
                                cout.flush();
                        }
                    }
                }
            }
        }
    }

    auto t2 = duration_cast<chrono::milliseconds>(chrono::system_clock::now().time_since_epoch()).count();

    if (progress)
        cout << "time elapsed: " << (t2 - t1) / 1000 << endl;
}

int main(int argc, char *argv[]) {
    mode selected = mode::onePair;

    int width, height, depth;
    string output;
    bool progress = false;

    auto defaults = (
            required("-w") & value("width", width),
                    required("-h") & value("height", height),
                    required("-d") & value("depth", depth)
    );

    Coordinate power{};
    Coordinate ground{};

    auto onePair = (
            command("onePair").set(selected, mode::onePair),
                    defaults,
                    required("-px") & value("power.x", power.x),
                    required("-py") & value("power.y", power.y),
                    required("-pz") & value("power.z", power.z),
                    required("-gx") & value("ground.x", ground.x),
                    required("-gy") & value("ground.y", ground.y),
                    required("-gz") & value("ground.z", ground.z)
    );

    auto fullGrid = (
            command("fullGrid").set(selected, mode::fullGrid),
                    defaults,
                    (required("-o", "--output") & value("outfile", output)) % "write to file instead of stdout",
                    option("--progress", "-p").set(progress) % "show progress"
    );

    auto cli = (
            (onePair | fullGrid | command("help").set(selected, mode::help)),
                    option("-v", "--version").call([] { cout << "version 1.0\n\n"; }).doc("show version"));

    if (parse(argc, argv, cli)) {
        switch (selected) {
            case mode::onePair:
                runOnePair(width, height, depth, power, ground);
                break;
            case mode::fullGrid:
                runFullGrid(width, height, depth, output, progress);
                break;
            case mode::help:
                cout << make_man_page(cli, "rgridtool");
                break;
        }
    } else {
        cout << usage_lines(cli, "rgridtool") << '\n';
    }
}