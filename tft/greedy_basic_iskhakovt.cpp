#include "strategies.h"

#include <set>
#include <vector>

using namespace std;

const int INF = 1e9 + 7;

int get_best_ride(const Game& game, const Data& data, int car, const set<int>& rides) {
    int minTime = INF, minRide = -1;
    for (int ride : rides) {
        if (game.cars[car].canGetTo(data.rides[ride])) {
            if (game.cars[car].timeOfFinish(data.rides[ride]) > data.T) {
                // Can't finish ride.
                continue;
            }

            int time = game.cars[car].timeOfStart(data.rides[ride]);

            if (minRide == -1 || time < minTime) {
                minRide = ride;
                minTime = time;
            }
        }
    }

    return minRide;
}

Game greedy_basic_thefacetakt(const Data& data) {
    int CARS = data.F;
    int RUNS = data.N;

    Game game(CARS);

    set<int> rides;
    for (int i = 0; i != RUNS; ++i) {
        rides.insert(i);
    }

    vector<int> next(CARS, -1);

    set<pair<int, int>> candidates; // timeToFinish, car

    for (int i = 0; i < CARS; ++i) {
        candidates.insert({0, i});
    }

    while (!rides.empty() && !candidates.empty()) {
        int car = candidates.begin()->second;
        int ride = get_best_ride(game, data, car, rides);
        if (ride == -1) {
            continue;
        }
        rides.erase(ride);
        game.push_ride(car, data.rides[ride], data.B);
        candidates.insert({game.cars[car].time, car});
    }

    return game;
}
