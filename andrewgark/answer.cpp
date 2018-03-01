#include "answer.h"
#include <cmath>
#include <algorithm>

int getRandomInt(const int mod) {
    int r = rand() % mod;
    //std::cerr << "random " << mod << ": " << r << std::endl;
    return r;
}

long double getRandomDouble(const long double mod) {
    return ((long double)rand() / (long double)(RAND_MAX));
}

long double getTemperatureProbLimit(const long double neighborBonus, const long double lastBonus, const long double temperature) {
    return exp(-(lastBonus - neighborBonus) / temperature);
}

Answer::Answer(const int N, const int F, const std::vector<std::vector<int>>& CarToRides, const std::vector<int>& RideToCar)
    : N(N)
    , F(F)
    , CarToRides(CarToRides)
    , RideToCar(RideToCar) {}


Answer Answer::read(std::istream& in, const int N, const int F) {
    std::vector<int> rideToCar(N, -1); // car F is no car
    std::vector<std::vector<int>> carToRides(F + 1);

    for (int car = 0; car != F; ++car) {
        int M;
        in >> M;
        for (int j = 0; j != M; ++j) {
            int ride;
            in >> ride;
            rideToCar[ride] = car;
            carToRides[car].push_back(ride);
        }
    }

    for (int ride = 0; ride != N; ++ride) {
        if (rideToCar[ride] == -1) {
            carToRides[F].push_back(ride);
        }
    }

    return Answer(N, F, carToRides, rideToCar);
}

Answer Answer::getRandom(const Data& data) {
    int N = data.N;
    int F = data.F;
    std::vector<int> rideToCar(N);
    std::vector<std::vector<int>> carToRides(F + 1);

    for (int ride = 0; ride != N; ++ride) {
        int randomCar = getRandomInt(F + 1);
        rideToCar[ride] = randomCar;
        carToRides[randomCar].push_back(ride);
    }
    for (int car = 0; car != F + 1; ++car) {
        random_shuffle(carToRides[car].begin(), carToRides[car].end());
    }

    return Answer(N, F, carToRides, rideToCar);
}

Answer Answer::getNeighbor(const int changes, const long double temperature) const {
    Answer neighborAnswer = *this;
    for (int i = 0; i < changes; ++i) {
        int firstRandomCar = getRandomInt(F + 1);
        int secondRandomCar = getRandomInt(F + 1);

        long double randomDouble = getRandomDouble(1.0);
        //std::vector<std::vector<int>>& firstCarToRides = neighborAnswer.CarToRides[firstRandomCar];
        //std::vector<std::vector<int>>& secondCarToRides = neighborAnswer.CarToRides[secondRandomCar];
        if (randomDouble < 0.5) { // change
            if (neighborAnswer.CarToRides[firstRandomCar].size() && neighborAnswer.CarToRides[secondRandomCar].size()) {
                int i = getRandomInt(neighborAnswer.CarToRides[firstRandomCar].size());
                int j = getRandomInt(neighborAnswer.CarToRides[secondRandomCar].size());
                std::swap(neighborAnswer.CarToRides[firstRandomCar][i], neighborAnswer.CarToRides[secondRandomCar][j]);
            }
        } else { // push inside
            if (neighborAnswer.CarToRides[firstRandomCar].size() && firstRandomCar != secondRandomCar) {
                int i = getRandomInt(neighborAnswer.CarToRides[firstRandomCar].size());
                int j = getRandomInt(neighborAnswer.CarToRides[secondRandomCar].size() + 1);

                int ride = neighborAnswer.CarToRides[firstRandomCar][i];
                neighborAnswer.CarToRides[firstRandomCar].erase(neighborAnswer.CarToRides[firstRandomCar].begin() + i);

                //std::cerr << neighborAnswer.CarToRides[secondRandomCar].size() << " " <<  j << " " << ride << std::endl;
                neighborAnswer.CarToRides[secondRandomCar].insert(neighborAnswer.CarToRides[secondRandomCar].begin() + j, ride);
                //std::cerr << "ok" << std::endl;
            }
        }

        //int firstRandomRide = getRandomInt(N);
        //int secondRandomRide = getRandomInt(N);
        //std::swap(neighborAnswer.RideToCar[firstRandomRide], neighborAnswer.RideToCar[secondRandomRide]);
        
    }
    return neighborAnswer;
}

// std::vector<std::set<int>> Answer::getCarToRides() {
//     std::vector<std::vector<int>> CarToRides(F);
//     for (int ride = 0; ride != N; ++ride) {
//         int car = RideToCar[ride];
//         if (car != -1) {
//             CarToRides[car].push_back(ride);
//         }
//     }
//     return CarToRides;
// }

long double Answer::getBonus(const Data& data) const {
    long long money = 0;
    for (int car = 0; car != F; ++car) {
        //std::cerr << "car " << car << " money " << money << " size " << CarToRides[car].size() << std::endl;

        coord_t coord = {0, 0};
        long long t = 0;
        for (int i = 0; i < CarToRides[car].size(); ++i) {
            int ride = CarToRides[car][i];

            //std::cerr << "ride " << ride << std::endl;
            auto start = data.rides[ride].start;
            auto  finish = data.rides[ride].finish;

            long long rdist = get_dist(coord, data.rides[ride].start);
            long long rtime = get_dist(data.rides[ride].start, data.rides[ride].finish);
            if (t + rdist <= data.rides[ride].earliestStart) {
                money += data.B;
            }
            t = std::max(t + rdist, (long long) data.rides[ride].earliestStart) + rtime;
            if (t <= data.T && t <= data.rides[ride].latestFinish) {
                money += rtime;
            }
        }
    }
    return money;
}

void Answer::print(std::ostream& out) {
    for (int car = 0; car != F; ++car) {
        out << CarToRides[car].size() << " ";
        for (auto it = CarToRides[car].begin(); it != CarToRides[car].end(); ++it) {
            out << *it << " ";
        }
        out << "\n";
    }
}