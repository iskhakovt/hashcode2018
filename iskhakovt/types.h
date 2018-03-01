#ifndef ISKHAKOVT_TYPES_H
#define ISKHAKOVT_TYPES_H

#include <cassert>
#include <cstdint>
#include <iostream>
#include <string>
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

    Data(int, int, int, int, int, int, std::vector<RideData>);

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

    const int id;

    RideData(coord_t, coord_t, int, int, int);

    static RideData read(std::istream&, int);

    int dist() const;
};


struct Game {
    std::vector<Car> cars;
    int64_t result = 0;
    int time = 0;

    explicit Game(int F) : cars(F) {}

    bool validate_output(const Data& data);
    void print(std::ostream&);

    void push_ride(int, const RideData&, int B);
};


struct Car {
    coord_t pos = {0, 0};

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

void print_percentage(int, int, const std::string& = "");
void decide_print_percentage(int, int, const std::string& = "");

#endif //ISKHAKOVT_TYPES_H
