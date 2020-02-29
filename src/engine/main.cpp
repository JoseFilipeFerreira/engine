#include "../deps/rapidxml.hpp"
#include <fstream>
#include <iostream>
#include <string.h>
#include <string>
#include <vector>

using namespace std;
using namespace rapidxml;

vector<string> read_models() {
    ifstream file;
    file.open("config.xml");
    string txt, buf;
    while(getline(file, buf))
        txt.append(buf);
    xml_document<> doc;
    char* txtm = (char*) malloc(txt.length() + 1);
    strcpy(txtm, txt.c_str());
    doc.parse<0>(txtm);
    vector<string> res;
    for(xml_node<> *n = doc.first_node()->first_node(); n; n = n->next_sibling()) 
        res.push_back(n->first_attribute()->value());
    return res;
}
