#ifndef PARSER_H
#define PARSER_H
#include"engine/model.hpp"

#include <vector>

class Group {
private:
  std::vector<Transform> transformations;
  std::vector<Model> models;
  Colour colour;
  std::vector<Group> subgroups;

public:
  Group();
  Group(std::vector<Transform> t, std::vector<Model> m, Colour c,
        std::vector<Group> g)
      : transformations(t), models(m), colour(c), subgroups(g) {}
  Group(std::string);
  void draw_group() const;
};


#endif // PARSER_H
