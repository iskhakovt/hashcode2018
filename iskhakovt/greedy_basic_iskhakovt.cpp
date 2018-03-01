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

Game greedy_basic_iskhakovt(const Data& data) {
    int CARS = data.F;
    int RUNS = data.N;

    Game game(CARS);

    set<int> rides;
    for (int i = 0; i != RUNS; ++i) {
        rides.insert(i);
    }

    vector<int> next;

    set<pair<int, pair<int, int>>> candidates; // timeToFinish, car, ride

    for (int car = 0; car != CARS; ++car) {
        int ride = get_best_ride(game, data, car, rides);
        if (ride != -1) {
            candidates.insert({game.cars[car].timeOfFinish(data.rides[ride]), {car, ride}});
        }
    }

    while (!candidates.empty()) {
        // for (const auto& p : candidates)

        int car = candidates.begin()->second.first;
        int ride = candidates.begin()->second.second;
        candidates.erase(candidates.begin());

        if (rides.find(ride) == rides.end()) continue;
        rides.erase(ride);

        game.cars[car].assignedRuns.push_back(ride);
        game.cars[car].time = game.cars[car].timeOfFinish(data.rides[ride]);

        ride = get_best_ride(game, data, car, rides);
        if (ride != -1) {
            candidates.insert({game.cars[car].timeOfFinish(data.rides[ride]), {car, ride}});
        }
    }

    return game;
}
