#ifndef MODEL_H
#define MODEL_H

#include "engine/transformations.hpp"
#include "utils/types.hpp"

#include <string>

class TerrainBuffer {
  private:
    GLuint _buffers[1];
    size_t _image_height, _image_width;

  public:
    TerrainBuffer(std::string const&, i32 , i32);
    void draw_terrain() const;
};

class ModelBuffer {
  private:
    GLuint _buffers[1];
    size_t _n_vertices;

  public:
    ModelBuffer(std::string const&);
    void draw_model() const;
};

class Object {
  private:
    std::string _file_name;
    Colour _colour;

  public:
    Object(const char*, Colour);
    void apply_colour() const { _colour.apply(); };
    auto model_name() const -> std::string const& { return _file_name; }
};

#endif // MODEL_H
