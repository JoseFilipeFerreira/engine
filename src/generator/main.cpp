#include "../utils/point.hpp"

#include "plane.hpp"
#include "box.hpp"
#include "sphere.hpp"
#include "cone.hpp"

#include<string>
#include<ostream>
#include<iostream>
#include<fstream>

template<typename T>
void file_writer(T v, std::string filename) {
    std::ofstream f;
    f.open(filename);
    for(Point p: v.draw())
        f << p.to_string() << "\n";
}

int main(int argc, char** argv) {
    std::string type(argv[1]);
    if("plane" == type)
        file_writer(Plane(argc - 3, argv + 2), argv[argc-1]);
    else if("box" == type)
        file_writer(Box(argc - 3, argv + 2), argv[argc-1]);
    else if("sphere" == type)
        file_writer(Sphere(argc - 3, argv + 2), argv[argc-1]);
    else if("cone" == type)
        file_writer(Cone(argc - 3, argv + 2), argv[argc-1]);
}
