#include "answer.h"
#include <cmath>

int getRandomInt(const int mod) {
    return rand() % mod;
}

long double getRandomDouble(const long double mod) {
    return ((long double)rand() / (long double)(RAND_MAX));
}

long double getTemperatureProbLimit(const long double neighborBonus, const long double lastBonus, const long double temperature) {
    return exp(-(lastBonus - neighborBonus) / temperature);
}

Answer::Answer(const int N, const int F, const std::vector<int>& RideToCar)
    : N(N)
    , F(F)
    , RideToCar(RideToCar) {}


Answer Answer::read(std::istream& in) {
    int N, F;
    in >> N >> F;
    std::vector<int> rideToCar(N, -1); // car -1 is no car

    for (int car = 0; car != F; ++car) {
        int M;
        in >> M;
        for (int j = 0; j != M; ++j) {
            int ride;
            in >> ride;
            rideToCar[ride - 1] = car;
        }
    }

    return Answer(N, F, rideToCar);
}

Answer Answer::getRandom(const Data& data) {
    int N = data.N;
    int F = data.F;
    std::vector<int> rideToCar(N);

    for (int ride = 0; ride != N; ++ride) {
        int randomCar = getRandomInt(F + 1) - 1;
        rideToCar[ride] = randomCar;
    }

    return Answer(N, F, rideToCar);
}

Answer Answer::getNeighbor(const int changes, const long double temperature) const {
    Answer neighborAnswer = *this;
    for (int i = 0; i < changes; ++i) {
        int firstRandomRide = getRandomInt(N);
        int secondRandomRide = getRandomInt(N);
        std::swap(neighborAnswer.RideToCar[firstRandomRide], neighborAnswer.RideToCar[secondRandomRide]);
    }
    return neighborAnswer;
}

long double Answer::getBonus(const Data& data) const {
    // TODO
}

void Answer::print(std::ostream&) {
    // TODO
}