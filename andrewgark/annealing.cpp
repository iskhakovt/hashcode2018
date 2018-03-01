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
const long double T_COEFF = 0.9999;
const int CHANGES = 5;
const int PRINT_STEP = 1000;
 
int main(int argc, char** argv) {
	srand(SEED);
    if (argc >= 2) {
        freopen(argv[1], "r", stdin);
    }

    Data data = Data::read(std::cin);

    Answer curAnswer;
    if (argc >= 3) {
        freopen(argv[2], "r", stdin);
    	curAnswer = Answer::read(std::cin, data.N, data.F);
    } else {
    	curAnswer = Answer::getRandom(data);
    }
	
	long double temperature = T_START;
    long double curBonus = curAnswer.getBonus(data);

    Answer bestAnswer = curAnswer;
    long double bestBonus = curBonus; 

    int i = 0;
    while (temperature > T_END) {
    	//std::cerr << i << " " << curBonus << std::endl;
    	Answer neighborAnswer = curAnswer.getNeighbor(CHANGES, temperature);

    	//std::cerr << "..." << std::endl;
    	long double neighborBonus = curAnswer.getBonus(data);

    	//std::cerr << "n: " << neighborBonus << std::endl;
        if ((neighborBonus > curBonus) || (getRandomDouble(1) < getTemperatureProbLimit(neighborBonus, curBonus, temperature))) {
        	curAnswer = neighborAnswer;
        	curBonus = neighborBonus;
        }

        if (curBonus > bestBonus) {
        	bestAnswer = curAnswer;
        	bestBonus = curBonus;
        }
 
        temperature *= T_COEFF;

        if (i % PRINT_STEP == 0) {
			freopen("result.txt", "w", stdout);
			bestAnswer.print(std::cout);
			freopen("result_bonus.txt", "w", stdout);
			cout << bestBonus;
        }
        i++;
    }
 	
	freopen("result.txt", "w", stdout);
	bestAnswer.print(std::cout);
	freopen("result_bonus.txt", "w", stdout);
	cout << bestBonus;

    return 0;
}
