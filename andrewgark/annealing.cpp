#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <cmath>
#include "../iskhakovt/types.cpp"
#include "../iskhakovt/types.h"
#include "answer.h"
#include "answer.cpp"
 
using namespace std;
 
const int SEED = 12345672;
const long double T_START = 1e9;
const long double T_END = 1e-9;
const long double T_COEFF = 0.9;
const int CHANGES = 5;
 
int main(int argc, char** argv) {
	srand(SEED);
    if (argc >= 2) {
        freopen(argv[1], "r", stdin);
    }

    Data data = Data::read(std::cin);

    Answer curAnswer;
    if (argc >= 3) {
        freopen(argv[2], "r", stdin);
    	curAnswer = Answer::read(std::cin);
    } else {
    	curAnswer = Answer::getRandom(data);
    }
	
	long double temperature = T_START;
 
    long double curBonus = curAnswer.getBonus(data);

    Answer bestAnswer = curAnswer;
    long double bestBonus = curBonus; 
    while (temperature > T_END)
    {
    	Answer neighborAnswer = curAnswer.getNeighbor(CHANGES, temperature);
    	long double neighborBonus = curAnswer.getBonus(data);
        if ((neighborBonus > curBonus) || (getRandomDouble(1) < getTemperatureProbLimit(neighborBonus, curBonus, temperature))) {
        	curAnswer = neighborAnswer;
        	curBonus = neighborBonus;
        }

        if (curBonus > bestBonus) {
        	bestAnswer = curAnswer;
        	bestBonus = curBonus;
        }
 
        temperature *= T_COEFF;
    }
 	

    return 0;
}
