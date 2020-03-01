#include<string>
#include<ostream>
#include<iostream>
#include<fstream>
using namespace std;

void file_writer(vector<Point> v, string filename)
{
    ofstream f;
    f.open(filename);
    for(Point p: v)
        f << p.to_string() << endl;
}

int main(int argc, char** argv)
{
    string type(argv[1]);
    Primitive* p;
    if("plane" == type)
        p = new Plane(argc - 3, argv + 2);
    else if("box" == type) {
        p = new Box(argc - 3, argv + 2);
    }
    else if("sphere" == type) {
        p = new Sphere(argc - 3, argv + 2);
    }
    else if("cone" == type) {
        p = new Cone(argc - 3, argv + 2);
    }
    file_writer(p->draw(), argv[argc-1]);
}
