#ifndef BUFFERS_H
#define BUFFERS_H

#include "engine/boundingBox.hpp"
#include "engine/light.hpp"
#include "utils/types.hpp"

#include <optional>
#include <unordered_map>
#include <vector>

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
    GLuint _points;
    GLuint _normals;
    GLuint _texture;

    size_t _image_height, _image_width;

    TerrainBuffer(
        GLuint points,
        GLuint normals,
        GLuint texture,
        size_t image_height,
        size_t image_width)
        : _points(points),
          _normals(normals),
          _texture(texture),
          _image_height(image_height),
          _image_width(image_width) {}

  public:
    static auto make(std::string const&, i32, i32)
        -> std::pair<TerrainBuffer, BoundingBox>;
    void draw_terrain() const;
};

class ModelBuffer {
  private:
    GLuint _points;
    GLuint _normals;
    GLuint _texture;

    size_t _n_points, _n_normals, _n_textures;

    ModelBuffer(
        GLuint points,
        GLuint normals,
        GLuint texture,
        size_t n_points,
        size_t n_normals,
        size_t n_textures)
        : _points(points),
          _normals(normals),
          _texture(texture),
          _n_points(n_points),
          _n_normals(n_normals),
          _n_textures(n_textures) {}

  public:
    static auto make(std::string const&) -> std::pair<ModelBuffer, BoundingBox>;
    void draw_model() const;
};

class GroupBuffer {
  private:
    std::unordered_map<std::string, ModelBuffer> _model_buffers;
    std::unordered_map<std::string, TerrainBuffer> _terrain_buffers;
    std::unordered_map<std::string, TextureBuffer> _texture_buffers;
    std::unordered_map<std::string, BoundingBox> _bounding_box;

  public:
    GroupBuffer();
    auto insert_model(std::string const&) -> BoundingBox;
    auto insert_terrain(std::string const&, i32, i32) -> BoundingBox;
    void insert_texture(std::string const&);
    void draw_model(std::string const&) const;
    void draw_terrain(std::string const&) const;
    void bind_texture(std::string const&) const;
    void unbind_texture() const;
};

#endif
