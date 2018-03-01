#include "types.h"

#include <cstdio>
#include <iostream>


int main(int argc, char** argv) {
    if (argc == 2) {
        freopen(argv[1], "r", stdin);
    }

    auto data = Data::read(std::cin);

    return 0;
}
