#ifndef ISKHAKOVT_TYPES_H
#define ISKHAKOVT_TYPES_H

#include <cassert>
#include <cstdint>
#include <iostream>
#include <vector>

typedef std::pair<int, int> coord_t;

struct Data;
struct RideData;
struct Game;
struct Car;

struct Data {
    const int R;
    const int C;
    const int F;
    const int N;
    const int B;
    const int T;
    const std::vector<RideData> rides;

    Data(int R, int C, int F, int N, int B, int T, std::vector<RideData> rides) :
            R(R),
            C(C),
            F(F),
            N(N),
            B(B),
            T(T),
            rides(rides) {
        assert(rides.size() == (size_t)N);
    }

    static Data read(std::istream&);
};


struct RideData {
    const coord_t start;
    const coord_t finish;

    const int earliestStart;
    const int latestFinish;
    // Legacy.
    const int s;
    const int f;

    RideData(coord_t start, coord_t finish, int _s, int _f) :
            start(start),
            finish(finish),
            earliestStart(_s),
            latestFinish(_f),
            s(_s),
            f(_f) {}

    static RideData read(std::istream&);

    int dist() const;
};


struct Game {
    std::vector<Car> cars;
    int64_t result = 0;
    int time = 0;

    explicit Game(int F) : cars(F) {}

    bool validate_output(const Data& data);
    void print(std::ostream&);
};


struct Car {
    coord_t pos = {0, 0};
    int ride = -1;

    // TODO

    std::vector<int> assignedRuns;
    int time = 0;

    Car() = default;

    int timeToStart(const RideData& ride) const;
    int timeOfStart(const RideData& ride) const;
    int timeToFinish(const RideData& ride) const;
    int timeOfFinish(const RideData& ride) const;
    bool canGetTo(const RideData& ride) const;
};


int get_dist(const coord_t&, const coord_t&);


#endif //ISKHAKOVT_TYPES_H
