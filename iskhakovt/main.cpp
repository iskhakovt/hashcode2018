#include "types.h"
#include "strategies.h"

#include <cstdio>
#include <iostream>


int main(int argc, char** argv) {
    if (argc >= 2) {
        freopen(argv[1], "r", stdin);
    }
    if (argc >= 3) {
        freopen(argv[2], "w", stdout);
    }

    auto data = Data::read(std::cin);

    auto game = greedy_basic_iskhakovt(data);

    game.print(std::cout);

    std::cerr << "SCORE: " << game.result << std::endl;

    return 0;
}
