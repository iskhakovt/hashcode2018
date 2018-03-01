#include "strategies.h"

#include <algorithm>
#include <set>
#include <vector>

using namespace std;

const int INF = 1e9 + 7;

vector<int> get_best_ride(const Game& game, const Data& data, int car, const vector<int>& rides) {
    vector<pair<int, int>> candidates; // ride, time

    for (int ride : rides) {
        if (game.usedRuns.find(ride) != game.usedRuns.end()) continue;

        if (game.cars[car].canGetTo(data.rides[ride])) {
            if (game.cars[car].timeOfFinish(data.rides[ride]) > data.T) {
                // Can't finish ride.
                continue;
            }

            int time = game.cars[car].timeOfStart(data.rides[ride]);

            bool pushed = false;
            for (int pos = 0; pos != candidates.size(); ++pos) {
                if (candidates[pos].second > time) {
                    candidates.insert(candidates.begin() + pos, {ride, time});
                    pushed = true;
                    break;
                }
            }

            if (pushed) {
                if (candidates.size() > 2) candidates.pop_back();
            } else {
                if (candidates.size() != 2) candidates.emplace_back(ride, time);
            }
        }
    }

    vector<int> res;
    for (const auto& p : candidates) {
        res.push_back(p.first);
    }
    return res;
}

Game greedy_basic_iskhakovt(const Data& data) {
    int CARS = data.F;
    int RUNS = data.N;

    vector<int> rides;
    for (int i = 0; i != RUNS; ++i) {
        rides.push_back(i);
    }

    vector<Game> games {Game(CARS)};

    bool moved = true;
    int done = 0;
    while (moved) {
        decide_print_percentage(++done, RUNS, "runs");

        moved = false;
        vector<Game> newGames;

        for (const auto& game : games) {
            vector<pair<int, int>> cars_min;

            for (int i = 0; i != CARS; ++i) {
                int time = game.cars[i].time;

                bool pushed = false;
                for (int pos = 0; pos != cars_min.size(); ++pos) {
                    if (cars_min[pos].second > time) {
                        cars_min.insert(cars_min.begin() + pos, {i, time});
                        pushed = true;
                        break;
                    }
                }

                if (pushed) {
                    if (cars_min.size() > 2) cars_min.pop_back();
                } else {
                    if (cars_min.size() != 2) cars_min.emplace_back(i, time);
                }
            }

            vector<pair<int, int>> candidates; // timeToFinish, car, ride

            for (const auto&p : cars_min) {
                int car = p.first;
                auto r = get_best_ride(game, data, car, rides);

                for (const auto& ride : r) {
                    //candidates.emplace_back({car, ride});
                    auto gameCopy = game;
                    gameCopy.push_ride(car, data.rides[ride], data.B);
                    newGames.push_back(gameCopy);
                    moved = true;
                }
            }
        }

        sort(newGames.begin(), newGames.end(), [](const auto& a, const auto& b) {
            return a.result > b.result;
        });

        if (newGames.empty()) break;
        games = std::vector<Game>(newGames.begin(), newGames.begin() + min(newGames.size(), (size_t)10));
    }

    return games[0];
}
