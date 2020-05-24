#ifndef MODEL_H
#define MODEL_H

#include "engine/boundingBox.hpp"
#include "engine/buffers.hpp"
#include "engine/transformations.hpp"
#include "utils/types.hpp"

#include <optional>
#include <string>

template<bool is_model>
class Object {
  private:
    std::string _file_name;
    std::optional<std::string> _texture_name;
    Colour _colour;
    Colour _diffuse;
    Colour _specular;
    Colour _emissive;
    Colour _ambient;

    BoundingBox _bb;

  public:
    Object(
        const char* file_name,
        std::optional<std::string> texture_name,
        Colour c,
        Colour diff,
        Colour spec,
        Colour emiss,
        Colour amb,
        BoundingBox bb)
        : _file_name(file_name),
          _texture_name(texture_name),
          _colour(c),
          _diffuse(diff),
          _specular(spec),
          _emissive(emiss),
          _ambient(amb),
          _bb(bb){};

    void draw(GroupBuffer const& group_buffer, bool DEBUG) const {
        _colour.apply();

        _diffuse.set_diffuse();
        _specular.set_specular();
        _emissive.set_emissive();
        _ambient.set_ambient();

        if(_texture_name.has_value()){
                group_buffer.bind_texture(_texture_name.value());
        }

        if constexpr (is_model) {
            group_buffer.draw_model(_file_name);
        } else {
            group_buffer.draw_terrain(_file_name);
        }
        group_buffer.unbind_texture();

        if (DEBUG) _bb.draw();
    }

    auto is_visible() const -> bool { return _bb.is_visible(); }
};

#endif // MODEL_H
