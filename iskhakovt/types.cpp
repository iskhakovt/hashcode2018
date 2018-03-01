#include "types.h"


Data::Data(int R, int C, int F, int N, int B, int T, std::vector<RideData> rides) :
        R(R),
        C(C),
        F(F),
        N(N),
        B(B),
        T(T),
        rides(rides) {
    assert(rides.size() == (size_t)N);
}

Data Data::read(std::istream& in) {
    int R, C, F, N, B, T;
    in >> R >> C >> F >> N >> B >> T;

    std::vector<RideData> rides;
    for (int i = 0; i != N; ++i) {
        rides.emplace_back(RideData::read(in, i));
    }

    return Data(R, C, F, N, B, T, rides);
}

RideData::RideData(coord_t start, coord_t finish, int _s, int _f, int id) :
    start(start),
    finish(finish),
    earliestStart(_s),
    latestFinish(_f),
    s(_s),
    f(_f),
    id(id) {}

RideData RideData::read(std::istream& in, int id) {
    coord_t start, finish;
    int s, f;
    in >> start.first >> start.second >> finish.first >> finish.second >> s >> f;
    return RideData(start, finish, s, f, id);
}

int RideData::dist() const {
    return get_dist(start, finish);
}

int Car::timeToStart(const RideData& ride) const {
    int dist = get_dist(pos, ride.start);
    return std::max(dist, ride.earliestStart - time);
}

int Car::timeOfStart(const RideData& ride) const {
    return time + timeToStart(ride);
}

int Car::timeToFinish(const RideData& ride) const {
    return timeToStart(ride) + ride.dist();
}

int Car::timeOfFinish(const RideData& ride) const {
    return time + timeToFinish(ride);
}

bool Car::canGetTo(const RideData& ride) const {
    return timeOfFinish(ride) <= ride.latestFinish;
}

double Car::getValue(const RideData ride, int B) const {
    double rideCost = ride.dist() + (timeOfStart(ride) == ride.earliestStart ? B : 0);
    return rideCost / (double)(timeToFinish(ride));
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
            out << " " << ride;
        }
        out << "\n";
    }
}

void Game::push_ride(int car, const RideData& ride, int B) {
    assert(cars[car].canGetTo(ride));

    if (cars[car].timeOfStart(ride) == ride.earliestStart) {
        result += B;
    }

    usedRuns.insert(ride.id);

    result += ride.dist();
    cars[car].time = cars[car].timeOfFinish(ride);
    cars[car].pos = ride.finish;
    cars[car].assignedRuns.push_back(ride.id);
}

void print_percentage(int done, int all, const std::string& message) {
    double percentage = static_cast<double>(done) / static_cast<double>(all);
    if (!message.empty()) {
        std::cerr << message << ": ";
    }
    std::cerr << percentage * 100.0 << "   \r";
    std::cerr.flush();
}


void decide_print_percentage(int done, int all, const std::string& message) {
    if ((all < 1000) || (done % (all / 1000) == 0)) {
        print_percentage(done, all, message);
    }
}
