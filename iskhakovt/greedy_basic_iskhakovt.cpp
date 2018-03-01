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

    vector<int> next(CARS, -1);

    set<pair<int, int>> candidates; // timeToFinish, car

    for (int car = 0; car != CARS; ++car) {
        int ride = get_best_ride(game, data, car, rides);
        if (ride != -1) {
            candidates.insert({game.cars[car].timeOfFinish(data.rides[ride]), car});
            next[car] = ride;
        }
    }

    int done = 0;
    while (!candidates.empty()) {
        decide_print_percentage(++done, RUNS, "runs");

        int car = candidates.begin()->second;
        int ride = next[car];
        assert(ride != -1);
        candidates.erase(candidates.begin());

        if (rides.find(ride) == rides.end()) continue;
        rides.erase(ride);

        for (int ncar = 0; ncar != CARS; ++ncar) {
            if (ncar == car) continue;
            if (next[ncar] == ride) {
                int nride = get_best_ride(game, data, ncar, rides);
                if (nride != -1) {
                    candidates.insert({game.cars[ncar].timeOfFinish(data.rides[nride]), ncar});
                    next[ncar] = nride;
                }
            }
        }

        game.push_ride(car, data.rides[ride], data.B);

        ride = get_best_ride(game, data, car, rides);
        if (ride != -1) {
            candidates.insert({game.cars[car].timeOfFinish(data.rides[ride]), car});
            next[car] = ride;
        }
    }

    return game;
}
