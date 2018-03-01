#include "types.h"

Data Data::read(std::istream& in) {
    int R, C, F, N, B, T;
    in >> R >> C >> F >> N >> B >> T;

    std::vector<RideData> rides;
    for (int i = 0; i != N; ++i) {
        rides.emplace_back(RideData::read(in));
    }

    return Data(R, C, F, N, B, T, rides);
}

RideData RideData::read(std::istream& in) {
    coord_t start, finish;
    int s, f;
    in >> start.first >> start.second >> finish.first >> finish.second >> s >> f;
    return RideData(start, finish, s, f);
}
