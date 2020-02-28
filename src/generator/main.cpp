#include<string>
#include "./plane.hpp"
#include<ostream>
#include<iostream>
using namespace std;

int main(int argc, char** argv) {
    string type(argv[1]);
    Plane* p;
    if("plane" == type) {
        p = new Plane(argc - 2, argv + 2);
        for(Point l: p->draw()) {
            cout << l.to_string() << endl ;
        }
    }
    else if("box" == type) {}
    else if("sphere" == type) {}
    else if("cone" == type) {}
}
