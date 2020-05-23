#include "engine/group.hpp"

#include "utils/point.hpp"
#include "utils/types.hpp"

#include <stdexcept>

GroupBuffer::GroupBuffer() {
    _model_buffers = std::unordered_map<std::string, ModelBuffer>();
    _terrain_buffers = std::unordered_map<std::string, TerrainBuffer>();
    _model_buffers = std::unordered_map<std::string, ModelBuffer>();
}

void GroupBuffer::insert_model(std::string const& model_name) {
    auto search = _model_buffers.find(model_name);
    if (search == _model_buffers.end()) {
        auto model = ModelBuffer(model_name);
        _model_buffers.insert(std::make_pair(model_name, model));
    }
}

void GroupBuffer::insert_terrain(
    std::string const& terrain_name, i32 min_height, i32 max_height) {
    auto search = _terrain_buffers.find(terrain_name);
    if (search == _terrain_buffers.end()) {
        auto terrain = TerrainBuffer(terrain_name, min_height, max_height);
        _terrain_buffers.insert(std::make_pair(terrain_name, terrain));
    }
}

void GroupBuffer::insert_texture(std::string const& texture_name) {
    auto search = _texture_buffers.find(texture_name);
    if (search == _texture_buffers.end()) {
        auto texture = TextureBuffer(texture_name);
        _texture_buffers.insert(std::make_pair(texture_name, texture));
    }
}

void GroupBuffer::bind_texture(
    std::optional<std::string> const& texture_name) const {
    if (!texture_name.has_value()) return;
    auto search = _texture_buffers.find(texture_name.value());
    if (search != _texture_buffers.end())
        (search->second).bind_texture();
    else
        throw std::invalid_argument("Attempted to bind an unloaded texture");
}

void GroupBuffer::unbind_texture() const {
    glBindTexture(GL_TEXTURE_2D, 0);
};

void GroupBuffer::draw_model(std::string const& model_name) const {
    auto search = _model_buffers.find(model_name);
    if (search != _model_buffers.end())
        (search->second).draw_model();
    else
        throw std::invalid_argument("Attempted to draw an unloaded model");
}

void GroupBuffer::draw_terrain(std::string const& terrain_name) const {
    auto search = _terrain_buffers.find(terrain_name);
    if (search != _terrain_buffers.end())
        (search->second).draw_terrain();
    else
        throw std::invalid_argument("Attempted to draw an unloaded terrain");
}

auto GroupBuffer::bounding_box(std::string const& object_name) const
    -> BoundingBox {
    auto search_model = _model_buffers.find(object_name);
    auto search_terrain = _terrain_buffers.find(object_name);

    if (search_model != _model_buffers.end())
        return (search_model->second).bounding_box();
    else if (search_terrain != _terrain_buffers.end()) {
        return (search_terrain->second).bounding_box();
    } else
        throw std::invalid_argument(
            "Attempted to get an unloaded bounding box");
}

Group::Group() {
    transformations = std::vector<Transform>();
    models = std::vector<Object>();
    terrains = std::vector<Object>();
    colour = Colour();
    subgroups = std::vector<Group>();
}

void Group::draw_group(
    float elapsed, bool DEBUG, GroupBuffer const& group_buffer) const {
    for (auto const& t : transformations) t.apply(DEBUG, elapsed);
    for (auto const& l : lights) l.on();
    for (auto const& m : models) {
        if (m.bounding_box().is_visible()) {
            m.apply_colour();
            m.set_material();
            if (DEBUG) m.bounding_box().draw();
            group_buffer.bind_texture(m.texture_name());
            group_buffer.draw_model(m.model_name());
            group_buffer.unbind_texture();
        }
    }
    for (auto const& t : terrains) {
        if (t.bounding_box().is_visible()) {
            t.apply_colour();
            t.set_material();
            if (DEBUG) t.bounding_box().draw();
            group_buffer.bind_texture(t.texture_name());
            group_buffer.draw_terrain(t.model_name());
            group_buffer.unbind_texture();
        }
    }
    for (auto const& g : subgroups) {
        glPushMatrix();
        g.draw_group(elapsed, DEBUG, group_buffer);
        glPopMatrix();
    }
}
