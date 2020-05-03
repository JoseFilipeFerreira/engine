#ifndef PARSER_H
#define PARSER_H
#include "engine/model.hpp"
#include "utils/types.hpp"

#include <unordered_map>
#include <vector>

class Group;
class GroupBuffer;

class GroupBuffer {
  private:
    std::unordered_map<std::string, ModelBuffer> _model_buffers;
    std::unordered_map<std::string, TerrainBuffer> _terrain_buffers;

  public:
    GroupBuffer();
    void insert_model(std::string const&);
    void insert_terrain(std::string const&, i32, i32);
    void draw_model(std::string const&) const;
    void draw_terrain(std::string const&) const;
};

class Group {
  private:
    std::vector<Transform> transformations;
    std::vector<Object> models;
    std::vector<Object> terrains;
    Colour colour;
    std::vector<Group> subgroups;

  public:
    Group();
    Group(
        std::vector<Transform> t,
        std::vector<Object> m,
        std::vector<Object> ter,
        Colour c,
        std::vector<Group> g)
        : transformations(t), models(m), terrains(ter), colour(c), subgroups(g) {}
    Group(const char*, GroupBuffer&);
    void draw_group(float, bool, GroupBuffer const&) const;
};

#endif // PARSER_H
