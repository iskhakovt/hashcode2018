#pragma once

#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <cmath>
#include <vector>
#include <set>
#include "../iskhakovt/types.h"

int getRandomInt(const int mod);
long double getRandomDouble(const long double mod);
long double getTemperatureProbLimit(const long double neighborBonus, const long double lastBonus, const long double temperature);

struct Answer {
	int N, F;
    std::vector<int> RideToCar;
    std::vector<std::vector<int>> CarToRides;

    Answer() = default;
    Answer(const int N, const int F, const std::vector<std::vector<int>>& CarToRides, const std::vector<int>& RideToCar);
    static Answer read(std::istream&, int, int);
    void print(std::ostream&);
    
    static Answer getRandom(const Data&);

    long double getBonus(const Data& data) const;
    Answer getNeighbor(const int changes, const long double temperature) const;
};
