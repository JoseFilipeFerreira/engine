#ifndef PARSER_H
#define PARSER_H
#include "engine/model.hpp"

#include <unordered_map>
#include <vector>

class Group;
class GroupBuffer;

class GroupBuffer {
  private:
    std::unordered_map<std::string, ModelBuffer> _model_buffers;

  public:
    GroupBuffer();
    void insert(std::string const&);
    void draw_model(std::string) const;
};

class Group {
  private:
    std::vector<Transform> transformations;
    std::vector<Model> models;
    Colour colour;
    std::vector<Group> subgroups;

  public:
    Group();
    Group(
        std::vector<Transform> t,
        std::vector<Model> m,
        Colour c,
        std::vector<Group> g)
        : transformations(t), models(m), colour(c), subgroups(g) {}
    Group(const char*, GroupBuffer&);
    void draw_group(GroupBuffer const&) const;
};

#endif // PARSER_H
