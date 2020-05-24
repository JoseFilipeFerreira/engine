#include "engine/buffers.hpp"

#include "utils/types.hpp"

#include <IL/il.h>
#include <fstream>
#include <vector>

#ifdef __APPLE__
#    include <GLUT/glut.h>
#else
#    include <GL/glew.h>
#    include <GL/glut.h>
#endif

#include <stdexcept>

GroupBuffer::GroupBuffer() {
    _model_buffers = std::unordered_map<std::string, ModelBuffer>();
    _terrain_buffers = std::unordered_map<std::string, TerrainBuffer>();
    _model_buffers = std::unordered_map<std::string, ModelBuffer>();
    _bounding_box = std::unordered_map<std::string, BoundingBox>();
}

auto GroupBuffer::insert_model(std::string const& model_name) -> BoundingBox {
    auto search = _model_buffers.find(model_name);
    if (search == _model_buffers.end()) {
        auto [model, bb] = ModelBuffer::make(model_name);
        _model_buffers.insert(std::make_pair(model_name, model));
        _bounding_box.insert(std::make_pair(model_name, bb));
        return bb;
    } else {
        return _bounding_box[model_name];
    }
}

auto GroupBuffer::insert_terrain(
    std::string const& terrain_name, i32 min_height, i32 max_height)
    -> BoundingBox {
    auto search = _terrain_buffers.find(terrain_name);
    if (search == _terrain_buffers.end()) {
        auto [terrain, bb] =
            TerrainBuffer::make(terrain_name, min_height, max_height);
        _terrain_buffers.insert(std::make_pair(terrain_name, terrain));
        _bounding_box.insert(std::make_pair(terrain_name, bb));
        return bb;
    } else {
        return _bounding_box[terrain_name];
    }
}

void GroupBuffer::insert_texture(std::string const& texture_name) {
    auto search = _texture_buffers.find(texture_name);
    if (search == _texture_buffers.end()) {
        auto texture = TextureBuffer(texture_name);
        _texture_buffers.insert(std::make_pair(texture_name, texture));
    }
}

void GroupBuffer::bind_texture(std::string const& texture_name) const {
    auto search = _texture_buffers.find(texture_name);
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

TextureBuffer::TextureBuffer(std::string const& file_name) {
    unsigned int t;
    ilGenImages(1, &t);
    ilBindImage(t);
    ilLoadImage((ILstring) file_name.c_str());
    _image_width = ilGetInteger(IL_IMAGE_WIDTH);
    _image_height = ilGetInteger(IL_IMAGE_HEIGHT);
    ilConvertImage(IL_RGBA, IL_UNSIGNED_BYTE);
    u8 const* tex_data = ilGetData();

    glGenTextures(1, &_texture);

    glBindTexture(GL_TEXTURE_2D, _texture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

    glTexImage2D(
        GL_TEXTURE_2D,
        0,
        GL_RGBA,
        _image_width,
        _image_height,
        0,
        GL_RGBA,
        GL_UNSIGNED_BYTE,
        tex_data);
}

auto calc_height(
    u8 const* image_data,
    u64 image_width,
    u64 i,
    u64 j,
    i32 min_height,
    i32 max_height) -> float {
    i32 new_interval = max_height - min_height;
    return image_data[i * image_width + j] * new_interval / 255.0f + min_height;
}

auto compute_normal(
    u8 const* image_data,
    i32 image_width,
    i32 min_height,
    i32 max_height,
    i32 i,
    i32 j) -> Vector {
    auto p1 = Point(
        i,
        calc_height(image_data, image_width, i, j - 1, min_height, max_height),
        j - 1);
    auto p2 = Point(
        i,
        calc_height(image_data, image_width, i, j + 1, min_height, max_height),
        j + 1);
    auto p3 = Point(
        i - 1,
        calc_height(image_data, image_width, i - 1, j, min_height, max_height),
        j);
    auto p4 = Point(
        i + 1,
        calc_height(image_data, image_width, i + 1, j, min_height, max_height),
        j);

    auto v1 = Vector(p1, p2);
    auto v2 = Vector(p3, p4);

    return v1.cross(v2).normalize();
}

auto TerrainBuffer::make(
    std::string const& fileName, i32 min_height, i32 max_height)
    -> std::pair<TerrainBuffer, BoundingBox> {
    u32 t;
    ilGenImages(1, &t);
    ilBindImage(t);
    ilLoadImage((ILstring) fileName.c_str());
    ilConvertImage(IL_LUMINANCE, IL_UNSIGNED_BYTE);
    size_t _image_width = ilGetInteger(IL_IMAGE_WIDTH);
    size_t _image_height = ilGetInteger(IL_IMAGE_HEIGHT);
    u8 const* image_data = ilGetData();

    std::vector<float> point_vec;
    std::vector<float> normal_vec;
    std::vector<float> tex_vec;

    for (i32 i = 1; i < _image_height - 2; i++) {
        for (i32 j = 1; j < _image_width - 1; j++) {
            // location (i+1, j)
            auto n_normal = compute_normal(
                image_data, _image_width, min_height, max_height, i + 1, j);
            normal_vec.push_back(n_normal.x());
            normal_vec.push_back(n_normal.y());
            normal_vec.push_back(n_normal.z());

            tex_vec.push_back(i + 1);
            tex_vec.push_back(j);

            point_vec.push_back(i - _image_width * 0.5f + 1);
            point_vec.push_back(calc_height(
                image_data, _image_width, i + 1, j, min_height, max_height));
            point_vec.push_back(j - _image_height * 0.5f);

            // location (i, j)
            auto normal = compute_normal(
                image_data, _image_width, min_height, max_height, i, j);
            normal_vec.push_back(normal.x());
            normal_vec.push_back(normal.y());
            normal_vec.push_back(normal.z());

            tex_vec.push_back(i);
            tex_vec.push_back(j);

            point_vec.push_back(i - _image_width * 0.5f);
            point_vec.push_back(calc_height(
                image_data, _image_width, i, j, min_height, max_height));
            point_vec.push_back(j - _image_height * 0.5f);
        }
    }

    auto bb = BoundingBox(point_vec);

    GLuint buffers[3];

    glGenBuffers(3, buffers);

    glBindBuffer(GL_ARRAY_BUFFER, buffers[0]);
    glBufferData(
        GL_ARRAY_BUFFER,
        sizeof(float) * point_vec.size(),
        point_vec.data(),
        GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, buffers[1]);
    glBufferData(
        GL_ARRAY_BUFFER,
        sizeof(float) * normal_vec.size(),
        normal_vec.data(),
        GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, buffers[2]);
    glBufferData(
        GL_ARRAY_BUFFER,
        sizeof(float) * tex_vec.size(),
        tex_vec.data(),
        GL_STATIC_DRAW);

    return {
        TerrainBuffer(
            buffers[0], buffers[1], buffers[2], _image_height, _image_width),
        bb};
}

void TerrainBuffer::draw_terrain() const {
    glBindBuffer(GL_ARRAY_BUFFER, _points);
    glVertexPointer(3, GL_FLOAT, 0, 0);

    glBindBuffer(GL_ARRAY_BUFFER, _normals);
    glNormalPointer(GL_FLOAT, 0, 0);

    glBindBuffer(GL_ARRAY_BUFFER, _texture);
    glTexCoordPointer(2, GL_FLOAT, 0, 0);

    for (i32 i = 1; i < _image_height - 2; i++) {
        glDrawArrays(
            GL_TRIANGLE_STRIP,
            (_image_width - 2) * 2 * i,
            (_image_width - 2) * 2);
    }
}

auto ModelBuffer::make(std::string const& fileName)
    -> std::pair<ModelBuffer, BoundingBox> {
    float px, py, pz, vx, vy, vz, tx, ty;

    std::vector<float> point_vec;
    std::vector<float> normal_vec;
    std::vector<float> texture_vec;

    auto file = std::ifstream(fileName);
    while (file >> px >> py >> pz >> vx >> vy >> vz >> tx >> ty) {
        point_vec.push_back(px);
        point_vec.push_back(py);
        point_vec.push_back(pz);
        normal_vec.push_back(vx);
        normal_vec.push_back(vy);
        normal_vec.push_back(vz);
        texture_vec.push_back(tx);
        texture_vec.push_back(ty);
    }

    auto bb = BoundingBox(point_vec);

    auto n_points = point_vec.size();
    auto n_normals = normal_vec.size();
    auto n_textures = texture_vec.size();

    // load points
    GLuint buffers[3];
    glGenBuffers(3, buffers);

    glBindBuffer(GL_ARRAY_BUFFER, buffers[0]);
    glBufferData(
        GL_ARRAY_BUFFER,
        sizeof(float) * n_points,
        point_vec.data(),
        GL_STATIC_DRAW);

    // load normals
    glBindBuffer(GL_ARRAY_BUFFER, buffers[1]);
    glBufferData(
        GL_ARRAY_BUFFER,
        sizeof(float) * n_normals,
        normal_vec.data(),
        GL_STATIC_DRAW);

    // load textures
    glBindBuffer(GL_ARRAY_BUFFER, buffers[2]);
    glBufferData(
        GL_ARRAY_BUFFER,
        sizeof(float) * n_textures,
        texture_vec.data(),
        GL_STATIC_DRAW);

    return {
        ModelBuffer(
            buffers[0],
            buffers[1],
            buffers[2],
            n_points,
            n_normals,
            n_textures),
        bb};
}

void ModelBuffer::draw_model() const {
    glBindBuffer(GL_ARRAY_BUFFER, _points);
    glVertexPointer(3, GL_FLOAT, 0, 0);

    glBindBuffer(GL_ARRAY_BUFFER, _normals);
    glNormalPointer(GL_FLOAT, 0, 0);

    glBindBuffer(GL_ARRAY_BUFFER, _texture);
    glTexCoordPointer(2, GL_FLOAT, 0, 0);

    glDrawArrays(GL_TRIANGLES, 0, _n_points);
}
