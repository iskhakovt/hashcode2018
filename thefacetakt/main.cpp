#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <fstream>

struct Ride {
    int s;
    int f;
    int sx;
    int sy;
    int fx;
    int fy;
};


Ride readRide(std::istream& stream) {
    Ride ans;
    stream >> ans.sx >> ans.sy >> ans.fx >> ans.fy >> ans.s >> ans.f;
    return ans;
}


std::vector<Ride> rides;
int R, C, F, N, B, T;

void read(std::string filename) {
    std::ifstream in(filename);
    in >> R >> C >> F >> N >> B >> T;
    for (int i = 0; i < N; ++i) {
        rides.push_back(readRide(in));
    }
}

void readOutput(std::string filename) {
    std::ifstream in(filename);
    std::string line;

    for (int i = 0; i < F; ++i) {
        std::getline(in, line);

    }

}


int main(int argc, char** argv) {
    std::string test = argv[1];
    std::string output = argv[2];
    std::cout << "Hello, World!" << std::endl;
    return 0;
}