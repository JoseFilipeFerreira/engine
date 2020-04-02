#include "engine/parser.hpp"

#include "deps/tinyxml.hpp"
#include "utils/types.hpp"

#include <stack>
#include <stdexcept>

GroupBuffer::GroupBuffer() {
    _model_buffers = std::unordered_map<std::string, ModelBuffer>();
}

void GroupBuffer::insert(std::string const& model_name) {
    auto search = _model_buffers.find(model_name);
    if (search == _model_buffers.end()) {
        auto model = ModelBuffer(model_name);
        _model_buffers.insert(std::make_pair(model_name, model));
    }
}

void GroupBuffer::draw_model(std::string model_name) const {
    auto search = _model_buffers.find(model_name);
    if (search != _model_buffers.end())
        (search->second).draw_model();
    else
        throw std::invalid_argument("Attempted to draw an unloaded model");
}

Group::Group() {
    transformations = std::vector<Transform>();
    models = std::vector<Model>();
    colour = Colour();
    subgroups = std::vector<Group>();
}

auto parse_colour(TiXmlElement* elem, Colour colour) -> Colour {
    if (elem->Attribute("colour")) {
        u32 r, g, b, a;
        std::string_view s = elem->Attribute("colour");
        if (s.length() == 7) {
            std::sscanf(s.data(), "#%02x%02x%02x", &r, &g, &b);
            a = 255;
        } else {
            std::sscanf(s.data(), "#%02x%02x%02x%02x", &r, &g, &b, &a);
        }
        colour = Colour(r / 255.0f, g / 255.0f, b / 255.0f, a / 255.0f);
    }
    return colour;
}

Group Parser(TiXmlElement* root, Colour colour, GroupBuffer& gb) {
    std::vector<Transform> vTran;
    std::vector<Model> vMod;
    std::vector<Group> vGroup;

    for (TiXmlElement* elem = root->FirstChildElement(); elem != NULL;
         elem = elem->NextSiblingElement()) {
        std::string_view type = elem->Value();

        if (type == "translate") {
            float x = std::stof(elem->Attribute("X") ?: "0");
            float y = std::stof(elem->Attribute("Y") ?: "0");
            float z = std::stof(elem->Attribute("Z") ?: "0");
            vTran.push_back(Translate(x, y, z));
        } else if (type == "rotate") {
            float ang = std::stof(elem->Attribute("angle") ?: "0");
            float x = std::stof(elem->Attribute("axisX") ?: "0");
            float y = std::stof(elem->Attribute("axisY") ?: "0");
            float z = std::stof(elem->Attribute("axisZ") ?: "0");
            float time = std::stof(elem->Attribute("time") ?: "0");
            vTran.push_back(Rotate(ang, x, y, z, time));
        } else if (type == "scale") {
            float x = std::stof(elem->Attribute("X") ?: "1");
            float y = std::stof(elem->Attribute("Y") ?: "1");
            float z = std::stof(elem->Attribute("Z") ?: "1");
            vTran.push_back(Scale(x, y, z));
        } else if (type == "model") {
            vMod.push_back(
                Model(elem->Attribute("file"), parse_colour(elem, colour)));
            gb.insert(elem->Attribute("file"));
        } else {
            vGroup.push_back(Parser(elem, parse_colour(elem, colour), gb));
        }
    }
    return Group(std::move(vTran), std::move(vMod), colour, std::move(vGroup));
}

Group::Group(const char* fileName, GroupBuffer& gb) {
    TiXmlDocument doc(fileName);
    if (!doc.LoadFile()) {
        throw doc.ErrorDesc();
    }

    TiXmlElement* root = doc.FirstChildElement();
    if (!root) {
        throw "Failed to load file: No root element.";
    }

    *this = Parser(doc.FirstChildElement(), Colour(), gb);
}

void Group::draw_group(GroupBuffer const& group_buffer) const {
    auto elapsed = glutGet(GLUT_ELAPSED_TIME) / 1000.0f;
    for (auto const& t : transformations) t.apply(elapsed);
    for (auto const& m : models) {
        m.apply_colour();
        group_buffer.draw_model(m.model_name());
    }
    for (auto const& g : subgroups) {
        glPushMatrix();
        g.draw_group(group_buffer);
        glPopMatrix();
    }
}
