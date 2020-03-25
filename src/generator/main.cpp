#include "utils/point.hpp"

#include "generator/plane.hpp"
#include "generator/box.hpp"
#include "generator/sphere.hpp"
#include "generator/cone.hpp"
#include "generator/cylinder.hpp"
#include "generator/torus.hpp"

#include<string>
#include<ostream>
#include<iostream>
#include<fstream>

static auto const help_string =
"Usage: generator [PRIMITIVA] [ARGS] [FILE NAME]\n\n"
"PRIMITIVA  ARGS\n"
"plane      [side]\n"
"box        [x] [y] [z] [divisions]\n"
"sphere     [radius] [slices] [stacks]\n"
"cone       [radius] [height] [slices] [stacks]\n"
"cylinder   [radius] [height] [slices] [stacks]\n"
"torus      [radius] [ring radius] [stacks] [slices]");

template<typename T>
void file_writer(T v, std::string const& filename) {
    auto f = std::ofstream(filename);
    for(Point p: v.draw())
        f << p.to_string() << "\n";
}

int main(int argc, char** argv) {
    auto type = std::string_view(argv[1]);
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
    else if("torus" == type)
        file_writer(Torus(argc - 3, argv + 2), argv[argc-1]);
    else
        std::cout << help_string << std::endl;
}
