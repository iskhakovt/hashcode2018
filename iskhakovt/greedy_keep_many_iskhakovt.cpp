#include "strategies.h"

#include <algorithm>
#include <future>
#include <set>
#include <vector>

using namespace std;


vector<int> get_best_rides(const Game& game, const Data& data, int car) {
    vector<pair<int, int>> candidates; // ride, time

    for (int ride = 0; ride != data.N; ++ride) {
        if (game.usedRuns.find(ride) != game.usedRuns.end()) continue;

        if (game.cars[car].canGetTo(data.rides[ride])) {
            if (game.cars[car].timeOfFinish(data.rides[ride]) > data.T) {
                // Can't finish ride.
                continue;
            }

            int time = game.cars[car].timeOfStart(data.rides[ride]);

            bool pushed = false;
            for (int pos = 0; pos != (int)candidates.size(); ++pos) {
                if (candidates[pos].second > time) {
                    candidates.insert(candidates.begin() + pos, {ride, time});
                    pushed = true;
                    break;
                }
            }

            if (pushed) {
                if (candidates.size() > 10) candidates.pop_back();
            } else {
                if (candidates.size() != 10) candidates.emplace_back(ride, time);
            }
        }
    }

    vector<int> res;
    for (const auto& p : candidates) {
        res.push_back(p.first);
    }
    return res;
}

struct Candidate {
    int game;
    int car;
    int ride;
    int64_t result;

    Candidate(int game, int car, int ride, int64_t result) :
        game(game), car(car), ride(ride), result(result) {}

    bool operator<(Candidate const& c) const {
        return result > c.result;
    }
};

void processGame(const Data& data, int car, int ride, Game* game, set<pair<int, pair<int, int>>>* candidates) {
    set<int> carsToFix;
    vector<set<pair<int, pair<int, int>>>::iterator> toDel;

    for (auto it = candidates->begin(); it != candidates->end(); ++it) {
        if (it->second.first == car || it->second.second == ride) {
            carsToFix.insert(it->second.first);
            toDel.push_back(it);
        }
    }

    for (auto it : toDel) candidates->erase(it);

    game->push_ride(car, data.rides[ride], data.B);

    for (int nextCar : carsToFix) {
        auto nextRides = get_best_rides(*game, data, nextCar);
        for (int nextRide : nextRides) {
            candidates->insert({game->cars[nextCar].timeOfFinish(data.rides[nextRide]), {nextCar, nextRide}});
        }
    }
};

Game greedy_keep_many_iskhakovt(const Data& data) {
    int CARS = data.F;
    int RUNS = data.N;

    Game startGame(CARS);
    set<pair<int, pair<int, int>>> startCandidates;
    for (int nextCar = 0; nextCar != CARS; ++nextCar) {
        auto nextRides = get_best_rides(startGame, data, nextCar);
        for (int nextRide : nextRides) {
            startCandidates.insert({startGame.cars[nextCar].timeOfFinish(data.rides[nextRide]), {nextCar, nextRide}});
        }
    }

    vector<pair<Game, set<pair<int, pair<int, int>>>>> games {{startGame, startCandidates}};

    int done = 0;
    for (;;) {
        decide_print_percentage(++done, RUNS, "runs");

        vector<Candidate> gameCandidates;

        for (int i = 0; i != (int)games.size(); ++i) {
            const auto& game = games[i].first;
            auto cand = games[i].second;

            for (int j = 0; j != 10 && !cand.empty(); ++j) {
                int car = cand.begin()->second.first;
                int ride = cand.begin()->second.second;
                gameCandidates.emplace_back(i, car, ride, game.reward(car, data.rides[ride], data.B));
            }
        }

        if (gameCandidates.empty()) break;

        sort(gameCandidates.begin(), gameCandidates.end());
        gameCandidates.erase(gameCandidates.begin() + min(gameCandidates.size(), (size_t)10), gameCandidates.end());

        vector<pair<Game, set<pair<int, pair<int, int>>>>> newGames;
        for (const auto& cand : gameCandidates) {
            newGames.emplace_back(games[cand.game].first, games[cand.game].second);
        }

        std::vector<std::future<void>> threads;

        for (int i = 0; i != (int)gameCandidates.size(); ++i) {
            threads.emplace_back(std::async(
                    std::launch::async,
                    processGame,
                    std::cref(data),
                    gameCandidates[i].car,
                    gameCandidates[i].ride,
                    &newGames[i].first,
                    &newGames[i].second));
        }

        for (auto &thread : threads) {
            thread.wait();
        }

        games = newGames;
    }

    assert(!games.empty());
    return games[0].first;
}
