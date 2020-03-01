#include<string>
#include "./plane.hpp"
#include<ostream>
#include<iostream>
#include<fstream>
using namespace std;

template<typename T>
void file_writer(T v, string filename) {
    ofstream f;
    f.open(filename);
    for(Point p: v.draw())
        f << p.to_string() << endl;
}

int main(int argc, char** argv) {
    string type(argv[1]);
    if("plane" == type)
        file_writer(Plane(argc - 3, argv + 2), argv[argc-1]);
    else if("box" == type) {}
    else if("sphere" == type) {}
    else if("cone" == type) {}
}
