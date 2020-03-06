#include "utils/point.hpp"

#include "generator/plane.hpp"
#include "generator/box.hpp"
#include "generator/sphere.hpp"
#include "generator/cone.hpp"
#include "generator/cylinder.hpp"

#include<string>
#include<ostream>
#include<iostream>
#include<fstream>

static std::string help_String(
"Usage: generator [PRIMITIVA] [ARGS] [FILE NAME]\n\n"
"PRIMITIVA  ARGS\n"
"plane      [side]\n"
"box        [x] [y] [z] [divisions]\n"
"sphere     [radius] [slices] [stacks]\n"
"cone       [radius] [height] [slices] [stacks]\n"
"cylinder   [radius] [height] [slices] [stacks]");

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
    else if("cylinder" == type)
        file_writer(Cylinder(argc - 3, argv + 2), argv[argc-1]);
    else
        std::cout << help_String << std::endl;
}
