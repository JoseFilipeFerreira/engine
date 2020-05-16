#ifndef MODEL_H
#define MODEL_H

#include "engine/transformations.hpp"
#include "utils/types.hpp"

#include <optional>
#include <string>

class TextureBuffer {
  private:
    unsigned int _texture;
    size_t _image_height, _image_width;

  public:
    TextureBuffer(std::string const&);
    void bind_texture() const { glBindTexture(GL_TEXTURE_2D, _texture); };
};

class TerrainBuffer {
  private:
    GLuint _points[1];
    size_t _image_height, _image_width;

  public:
    TerrainBuffer(std::string const&, i32, i32);
    void draw_terrain() const;
};

class ModelBuffer {
  private:
    GLuint _points[1];
    GLuint _normals[1];
    GLuint _texture[1];

    size_t _n_points, _n_normals, _n_textures;

  public:
    ModelBuffer(std::string const&);
    void draw_model() const;
};

class Object {
  private:
    std::string _file_name;
    std::optional<std::string> _texture_name;
    Colour _colour;
    std::optional<Colour> _diffuse;
    std::optional<Colour> _specular;
    std::optional<Colour> _emissive;
    std::optional<Colour> _ambient;

  public:
    Object(
        const char* file_name,
        std::optional<std::string> texture_name,
        Colour c,
        std::optional<Colour> diff,
        std::optional<Colour> spec,
        std::optional<Colour> emiss,
        std::optional<Colour> amb)
        : _file_name(file_name),
          _texture_name(texture_name),
          _colour(c),
          _diffuse(diff),
          _specular(spec),
          _emissive(emiss),
          _ambient(amb){};
    void apply_colour() const { _colour.apply(); };
    void set_material() const;
    auto model_name() const -> std::string const& { return _file_name; }
    auto texture_name() const -> std::optional<std::string> const& {
        return _texture_name;
    }
};

#endif // MODEL_H
