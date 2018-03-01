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

int RideData::dist() const {
    return get_dist(start, finish);
}

int Car::timeOfStart(const RideData& ride) const {
    return get_dist(pos, ride.start);
}

int Car::timeOfFinish(const RideData& ride) const {
    int dist = get_dist(pos, ride.start);
    return std::min(time + dist, ride.earliestStart) + ride.dist();
}

bool Car::canGetTo(const RideData& ride) const {
    return timeOfFinish(ride) <= ride.latestFinish;
}

int get_dist(const coord_t& start, const coord_t& finish) {
    return std::max(start.first, finish.first) - std::min(start.first, finish.first) +
           std::max(start.second, finish.second) - std::min(start.second, finish.second);
}

bool Game::validate_output(const Data& data) {
    // TODO
    return true;
}

void Game::print(std::ostream& out) {
    for (int i = 0; i != (int)cars.size(); ++i) {
        out << cars[i].assignedRuns.size();
        for (int ride : cars[i].assignedRuns) {
            out << " " << ride + 1;
        }
        out << "\n";
    }
}
