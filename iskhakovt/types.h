//
// Created by Timur Iskhakov on 3/1/18.
//

#ifndef ISKHAKOVT_TYPES_H
#define ISKHAKOVT_TYPES_H

#include <cassert>
#include <cstdint>
#include <iostream>
#include <vector>

typedef std::pair<int, int> coord_t;

struct RideData;
struct Car;

struct Data {
    const int R;
    const int C;
    const int F;
    const int N;
    const int B;
    const int T;

    const std::vector<RideData> rides;

    int64_t result;

    Data(int R, int C, int F, int N, int B, int T, std::vector<RideData> rides) :
            R(R),
            C(C),
            F(F),
            N(N),
            B(B),
            T(T),
            rides(rides),
            result(0) {
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
};

struct Car {
    coord_t pos = {0, 0};
    int ride = -1;

    // TODO

    Car() = default;
};

#endif //ISKHAKOVT_TYPES_H
