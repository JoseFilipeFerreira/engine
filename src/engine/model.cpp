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

Object::Object(const char* fileName, Colour c) {
    _colour = c;
    _file_name = fileName;
}

TerrainBuffer::TerrainBuffer(
    std::string const& fileName, i32 min_height, i32 max_height) {
    u32 t;
    unsigned char* imageData;

    ilGenImages(1, &t);
    ilBindImage(t);
    ilLoadImage((ILstring) fileName.c_str());
    ilConvertImage(IL_LUMINANCE, IL_UNSIGNED_BYTE);
    // important: check tw and th values
    // both should be equal to 256
    // if not there was an error loading the image
    // most likely the image could not be found
    _image_width = ilGetInteger(IL_IMAGE_WIDTH);
    _image_height = ilGetInteger(IL_IMAGE_HEIGHT);
    // imageData is a LINEAR array with the pixel values
    imageData = ilGetData();

    // 	Build the vertex arrays
    std::vector<float> vec;
    i32 new_interval = max_height - min_height;
    for (i32 h = 0; h < _image_height; h++) {
        for (i32 w = 0; w < _image_width; w++) {
            float point = imageData[_image_height * h + w];
            float n_point = imageData[_image_height * (h + 1) + w];

            vec.push_back(w - (_image_width / 2.0));        // x1
            vec.push_back(point * new_interval / 255.0f + min_height);          // y1
            vec.push_back(h - (_image_height / 2.0));       // z1
            vec.push_back(w - (_image_width / 2.0));        // x2
            vec.push_back(n_point * new_interval / 255.0f + min_height);        // y2
            vec.push_back(h + 1.0 - (_image_height / 2.0)); // z2
        }
    }

    glGenBuffers(1, _buffers);
    glBindBuffer(GL_ARRAY_BUFFER, _buffers[0]);
    glBufferData(
        GL_ARRAY_BUFFER,
        sizeof(float) * vec.size(),
        vec.data(),
        GL_STATIC_DRAW);
}

void TerrainBuffer::draw_terrain() const {
    glBindBuffer(GL_ARRAY_BUFFER, _buffers[0]);
    glVertexPointer(3, GL_FLOAT, 0, 0);
    for (i32 i = 0; i < _image_height - 1; i++) {
        glDrawArrays(GL_TRIANGLE_STRIP, i * 2 * _image_width, _image_width * 2);
    }
}

ModelBuffer::ModelBuffer(std::string const& fileName) {
    float x, y, z;
    std::vector<float> vec;
    auto file = std::ifstream(fileName);
    while (file >> x >> y >> z) {
        vec.push_back(x);
        vec.push_back(y);
        vec.push_back(z);
    }
    _n_vertices = vec.size();

    glGenBuffers(1, _buffers);
    glBindBuffer(GL_ARRAY_BUFFER, _buffers[0]);
    glBufferData(
        GL_ARRAY_BUFFER,
        sizeof(float) * _n_vertices,
        vec.data(),
        GL_STATIC_DRAW);
}

void ModelBuffer::draw_model() const {
    glBindBuffer(GL_ARRAY_BUFFER, _buffers[0]);
    glVertexPointer(3, GL_FLOAT, 0, 0);
    glDrawArrays(GL_TRIANGLES, 0, _n_vertices);
}
