#include "engine/parser.hpp"

#include "deps/tinyxml.hpp"
#include "utils/types.hpp"

Group::Group() {
  transformations = std::vector<Transform>();
  models = std::vector<Model>();
  colour = Colour();
  subgroups = std::vector<Group>();
}

Colour parse_colour(TiXmlElement *elem, Colour colour) {
  if (elem->Attribute("colour")) {
    u32 r, g, b, a;
    std::string s = elem->Attribute("colour");
    if (s.length() == 7) {
      std::sscanf(s.c_str(), "#%02x%02x%02x", &r, &g, &b);
      a = 255;
    } else
      std::sscanf(s.c_str(), "#%02x%02x%02x%02x", &r, &g, &b, &a);

    colour = Colour(static_cast<float>(r)/255, static_cast<float>(g)/255, static_cast<float>(b)/255, static_cast<float>(a)/255);
  }
  return colour;
}

Group Parser(TiXmlElement *root, Colour colour) {
  std::vector<Transform> vTran;
  std::vector<Model> vMod;
  std::vector<Group> vGroup;

  for (TiXmlElement *elem = root->FirstChildElement(); elem != NULL;
       elem = elem->NextSiblingElement()) {
    std::string type = elem->Value();

    if (type == "translate") {
      float x = std::stof(elem->Attribute("X") ?: "0");
      float y = std::stof(elem->Attribute("Y") ?: "0");
      float z = std::stof(elem->Attribute("Z") ?: "0");
      vTran.push_back(Translate(x, y, z));
    } else if (type == "rotate") {
      float ang = std::stof(elem->Attribute("angle") ?: "0");
      float x = std::stof(elem->Attribute("axisX") ?: "0");
      float y = std::stof(elem->Attribute("axisY") ?: "0");
      float z = std::stof(elem->Attribute("axisZ") ?: "0");
      vTran.push_back(Rotate(ang, x, y, z));
    } else if (type == "scale") {
      float x = std::stof(elem->Attribute("X") ?: "1");
      float y = std::stof(elem->Attribute("Y") ?: "1");
      float z = std::stof(elem->Attribute("Z") ?: "1");
      vTran.push_back(Scale(x, y, z));
    } else if (type == "model") {
      vMod.push_back(
          Model(elem->Attribute("file"), parse_colour(elem, colour)));
    } else {
      vGroup.push_back(Parser(elem, parse_colour(elem, colour)));
    }
  }
  return Group(vTran, vMod, colour, vGroup);
}

Group::Group(std::string fileName) {
  TiXmlDocument doc(fileName);
  if (!doc.LoadFile()) {
    std::cerr << doc.ErrorDesc() << std::endl;
    return;
  }

  TiXmlElement *root = doc.FirstChildElement();
  if (root == NULL) {
    std::cerr << "Failed to load file: No root element." << std::endl;
    doc.Clear();
    return;
  }

  *this = Parser(doc.FirstChildElement(), Colour());
}

void Group::draw_group() const {
  for (auto &t : transformations)
    t.apply();
  for (auto &m : models)
    m.draw_model();
  for (auto &g : subgroups) {
    glPushMatrix();
    g.draw_group();
    glPopMatrix();
  }
}
