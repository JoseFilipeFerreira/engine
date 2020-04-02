#include "engine/model.hpp"

#include "utils/types.hpp"

#include <fstream>
#include <iostream>

Model::Model(const char* fileName, Colour c) {
    _colour = c;
    _file_name = fileName;
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
