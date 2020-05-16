#include "engine/model.hpp"

#include <IL/il.h>
#include <fstream>
#include <vector>

#ifdef __APPLE__
#    include <GLUT/glut.h>
#else
#    include <GL/glew.h>
#    include <GL/glut.h>
#endif

TextureBuffer::TextureBuffer(std::string const& file_name) {
    unsigned int t;
    unsigned char* texData;
    ilGenImages(1, &t);
    ilBindImage(t);
    ilLoadImage((ILstring) file_name.c_str());
    _image_width = ilGetInteger(IL_IMAGE_WIDTH);
    _image_height = ilGetInteger(IL_IMAGE_HEIGHT);
    ilConvertImage(IL_RGBA, IL_UNSIGNED_BYTE);
    texData = ilGetData();

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
        texData);
}

TerrainBuffer::TerrainBuffer(
    std::string const& fileName, i32 min_height, i32 max_height) {
    u32 t;
    unsigned char* imageData;

    ilGenImages(1, &t);
    ilBindImage(t);
    ilLoadImage((ILstring) fileName.c_str());
    ilConvertImage(IL_LUMINANCE, IL_UNSIGNED_BYTE);
    _image_width = ilGetInteger(IL_IMAGE_WIDTH);
    _image_height = ilGetInteger(IL_IMAGE_HEIGHT);
    imageData = ilGetData();

    // 	Build the vertex arrays
    std::vector<float> vec;
    i32 new_interval = max_height - min_height;
    for (i32 h = 0; h < _image_height; h++) {
        for (i32 w = 0; w < _image_width; w++) {
            float point = imageData[_image_height * h + w];
            float n_point = imageData[_image_height * (h + 1) + w];

            vec.push_back(w - (_image_width / 2.0));                     // x1
            vec.push_back(point * new_interval / 255.0f + min_height);   // y1
            vec.push_back(h - (_image_height / 2.0));                    // z1
            vec.push_back(w - (_image_width / 2.0));                     // x2
            vec.push_back(n_point * new_interval / 255.0f + min_height); // y2
            vec.push_back(h + 1.0 - (_image_height / 2.0));              // z2
        }
    }

    glGenBuffers(1, _points);
    glBindBuffer(GL_ARRAY_BUFFER, _points[0]);
    glBufferData(
        GL_ARRAY_BUFFER,
        sizeof(float) * vec.size(),
        vec.data(),
        GL_STATIC_DRAW);
}

void TerrainBuffer::draw_terrain() const {
    glBindBuffer(GL_ARRAY_BUFFER, _points[0]);
    glVertexPointer(3, GL_FLOAT, 0, 0);
    for (i32 i = 0; i < _image_height - 1; i++) {
        glDrawArrays(GL_TRIANGLE_STRIP, i * 2 * _image_width, _image_width * 2);
    }
}

ModelBuffer::ModelBuffer(std::string const& fileName) {
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
    _n_points = point_vec.size();
    _n_normals = normal_vec.size();
    _n_textures = texture_vec.size();

    // load points
    glGenBuffers(1, _points);
    glBindBuffer(GL_ARRAY_BUFFER, _points[0]);
    glBufferData(
        GL_ARRAY_BUFFER,
        sizeof(float) * _n_points,
        point_vec.data(),
        GL_STATIC_DRAW);

    // load normals
    glGenBuffers(1, _normals);
    glBindBuffer(GL_ARRAY_BUFFER, _normals[0]);
    glBufferData(
        GL_ARRAY_BUFFER,
        sizeof(float) * _n_normals,
        normal_vec.data(),
        GL_STATIC_DRAW);

    // load textures
    glGenBuffers(1, _texture);
    glBindBuffer(GL_ARRAY_BUFFER, _texture[0]);
    glBufferData(
        GL_ARRAY_BUFFER,
        sizeof(float) * _n_textures,
        texture_vec.data(),
        GL_STATIC_DRAW);
}

void ModelBuffer::draw_model() const {
    glBindBuffer(GL_ARRAY_BUFFER, _points[0]);
    glVertexPointer(3, GL_FLOAT, 0, 0);

    glBindBuffer(GL_ARRAY_BUFFER, _normals[0]);
    glNormalPointer(GL_FLOAT, 0, 0);

    glBindBuffer(GL_ARRAY_BUFFER, _texture[0]);
    glTexCoordPointer(2, GL_FLOAT, 0, 0);

    glDrawArrays(GL_TRIANGLES, 0, _n_points);
}

void Object::set_material() const {
    if (_diffuse.has_value()) {
        _diffuse.value().set_diffuse();
    }
    if (_specular.has_value()) {
        _specular.value().set_diffuse();
    }
    if (_emissive.has_value()) {
        _emissive.value().set_diffuse();
    }
    if (_ambient.has_value()) {
        _ambient.value().set_diffuse();
    }
}
