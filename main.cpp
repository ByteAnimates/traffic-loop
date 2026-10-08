// The demo. Build and run:
//
//     c++ -std=c++17 -O2 -o main main.cpp
//     ./main
//
// `signal.cpp` is the snippet from the reel, generated from the episode. It is INCLUDED rather
// than linked because it is exactly what was on screen: one function with no headers of its own.
// This file supplies what it uses — the two lights, `wait`, and the `Loop` — and then runs it.
//
// The snippet is a MODEL of an actuated traffic signal, not a controller. A real one latches a
// call, runs minimum and maximum greens, and sets the yellow from the road's speed; this one
// keeps only the two rules the reel is about:
//
//   1. the side street's light changes only when the loop in the road asks for it, and
//   2. no one gets a green until every light has been red for a moment (the all-red interval).
//
// THE LOOP'S NUMBERS ARE ILLUSTRATIVE. A loop detector feels conductive metal over it, not
// weight. How much it feels depends on the vehicle, the loop and its sensitivity setting, and
// is not one number per kind of vehicle. The units below are made up for the demo, and say only
// what the reel says: a car is far over the line, and a bike with little metal can be under it.

#include <cstdlib>
#include <iostream>
#include <string>
#include <vector>

using namespace std;

enum Light { RED, YELLOW, GREEN };

Light road = GREEN;  // the main road's light
Light side = RED;    // the side street's light

/** Every `wait` records what both lights showed, and for how long. */
struct Interval {
    Light road, side;
    int seconds;
};
static vector<Interval> timeline;

void wait(int seconds) { timeline.push_back({road, side, seconds}); }

struct Loop {
    double metal;  // illustrative units — see the top of this file
    static constexpr double LINE = 0.4;
    bool has_metal() const { return metal >= LINE; }
};

#include "signal.cpp"

/** A check that survives `-DNDEBUG`: this demo's output is a claim, so a failure stops it. */
static void require(bool ok, const string& what) {
    if (!ok) {
        cerr << "FAILED: " << what << "\n";
        exit(1);
    }
}

static string name(Light l) { return l == RED ? "red" : l == YELLOW ? "yellow" : "green"; }

/** Run `serve` once from rest with `metal` over the loop, print what the lights did, and check it. */
static void run(const string& what, double metal, bool expect_change) {
    road = GREEN;
    side = RED;
    timeline.clear();

    Loop loop{metal};
    serve(loop);

    cout << what << " (metal " << metal << ", line " << Loop::LINE << "):\n";
    if (timeline.empty()) cout << "  no call: main road stays " << name(road) << "\n";
    for (const auto& i : timeline)
        cout << "  main " << name(i.road) << ", side " << name(i.side) << "  for " << i.seconds << " s\n";
    cout << "  then main " << name(road) << ", side " << name(side) << "\n\n";

    require(!timeline.empty() == expect_change, what + ": the light changed when it should not have, or did not when it should");
    require(road == GREEN && side == RED, what + ": serve() must leave the junction at rest");

    // Never both green, and every green handed out of an all-red interval.
    Light pr = GREEN, ps = RED;
    for (const auto& i : timeline) {
        require(!(i.road == GREEN && i.side == GREEN), what + ": both directions green");
        if (i.side == GREEN && ps != GREEN) require(pr == RED && ps == RED, what + ": side green without all-red first");
        pr = i.road;
        ps = i.side;
    }
    if (!timeline.empty()) require(pr == RED && ps == RED, what + ": main green without all-red first");
}

int main() {
    run("nothing on the loop", 0.0, false);
    run("a car on the loop", 1.0, true);
    run("a bike on the loop", 0.12, false);
    cout << "all checks passed\n";
}
