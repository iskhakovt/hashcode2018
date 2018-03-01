#include "strategies.h"

#include <set>
#include <vector>

using namespace std;

const int INF = 1e9 + 7;
constexpr int BEAM_SIZE = 10;

double time_metric(const Game& game, const Data& data, int car, int ride) {
    int money = 0;
    if (game.cars[car].timeOfStart(data.rides[ride]) == data.rides[ride].earliestStart) {
        money += data.B;
    }

    int time = game.cars[car].timeToFinish(data.rides[ride]);

    double cur = -1.0 * money / time;
    return cur;
}

double time_div_dist_metric(const Game& game, const Data& data, int car, int ride) {
    return game.cars[car].timeOfStart(data.rides[ride]);
}

double get_metric(const Game& game, const Data& data, int car, int ride) {
    return time_metric(game, data, car, ride);

}

std::vector<int> get_best_ride(const Game& game, const Data& data, int car, const set<int>& rides, int topn=1) {
    std::set<std::pair<double, int>> top;
    for (int ride : rides) {
        if (game.cars[car].canGetTo(data.rides[ride])) {
            if (game.cars[car].timeOfFinish(data.rides[ride]) > data.T) {
                // Can't finish ride.
                continue;
            }

            double cur = get_metric(game, data, car, ride);
            if (top.size() < topn) {
                top.insert({cur, ride});
            }  else if (top.rbegin()->first > cur) {
                top.erase(std::prev(top.end()));
                top.insert({cur, ride});
            }
        }
    }

    std::vector<int> res;
    for (auto x: top) {
        res.push_back(x.second);
    }

    return res;
}

struct SuperGame {
    Game game;
    set<pair<int, int>> candidates; // timeToFinish, car
    set<int> rides;
};

void clean(std::set<std::pair<int64_t, SuperGame>> &games, std::vector<Game> &endgames) {
    bool f = true;
    while (f) {
        f = false;
        for (auto it = games.begin(); it != games.end(); ++it) {
            if (it->second.rides.empty() || it->second.candidates.empty()) {
                endgames.push_back(it->second.game);
                games.erase(it);
                f = true;
                break;
            }
        }
    }
}

Game bimsearch_tft(const Data& data) {
    int CARS = data.F;
    int RUNS = data.N;

    std::set<std::pair<int64_t, SuperGame>> games;

    set<int> rides;
    for (int i = 0; i != RUNS; ++i) {
        rides.insert(i);
    }

    set<pair<int, int>> candidates;
    for (int i = 0; i < CARS; ++i) {
        candidates.insert({0, i});
    }
    Game game(CARS);
    games.insert({game.result, {game, candidates, rides}});
    std::vector<Game> endgames;

    while (!games.empty()) {
        clean(games, endgames);
        if (games.empty()) {
            break;
        }
        for (auto& g: games) {
            int car = g.second.candidates.begin()->second;
            g.second.candidates.erase(candidates.begin());
            auto rrides = get_best_ride(g.second.game, data, car, rides);
        }
    }

    while (!rides.empty() && !candidates.empty()) {
        int car = candidates.begin()->second;
        candidates.erase(candidates.begin());
        auto rides = get_best_ride(game, data, car, rides);
        if (ride == -1) {
            continue;
        }
        rides.erase(ride);
        game.push_ride(car, data.rides[ride], data.B);
        candidates.insert({game.cars[car].time, car});
    }

    return game;
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
        candidates.erase(candidates.begin());
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
