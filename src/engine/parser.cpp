#include "engine/parser.hpp"

#include "deps/tinyxml.hpp"
#include "utils/point.hpp"
#include "utils/types.hpp"

#include <stack>
#include <stdexcept>

GroupBuffer::GroupBuffer() {
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
        throw std::invalid_argument("Attempted to draw an unloaded model");
}

Group::Group() {
    transformations = std::vector<Transform>();
    models = std::vector<Object>();
    terrains = std::vector<Object>();
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

auto parse_points(TiXmlElement* root) -> std::vector<Point> {
    std::vector<Point> points;
    for (TiXmlElement* elem = root->FirstChildElement(); elem != NULL;
         elem = elem->NextSiblingElement()) {
        std::string_view type = elem->Value();
        if (type == "point") {
            float x = std::stof(elem->Attribute("X") ?: "0");
            float y = std::stof(elem->Attribute("Y") ?: "0");
            float z = std::stof(elem->Attribute("Z") ?: "0");
            points.push_back(Point(x, y, z));
        }
    }

    return points;
}

Group Parser(TiXmlElement* root, Colour colour, GroupBuffer& gb) {
    std::vector<Transform> vTran;
    std::vector<Object> vMod;
    std::vector<Object> vTer;
    std::vector<Group> vGroup;

    for (TiXmlElement* elem = root->FirstChildElement(); elem != NULL;
         elem = elem->NextSiblingElement()) {
        std::string_view type = elem->Value();

        if (type == "translate") {
            if (elem->Attribute("time")) {
                float const time = std::stof(elem->Attribute("time") ?: "0");
                auto point_vec = parse_points(elem);
                if (point_vec.size() < 4)
                    throw std::invalid_argument(
                        "There need to be at least 4 points in a Catmull-Ron curve");
                vTran.push_back(CatmullRon(time, point_vec));
            } else {
                float x = std::stof(elem->Attribute("X") ?: "0");
                float y = std::stof(elem->Attribute("Y") ?: "0");
                float z = std::stof(elem->Attribute("Z") ?: "0");
                vTran.push_back(Translate(x, y, z));
            }
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
                Object(elem->Attribute("file"), parse_colour(elem, colour)));
            gb.insert_model(elem->Attribute("file"));
        } else if (type == "terrain") {
            float min_height = std::stof(elem->Attribute("min") ?: "0");
            float max_height = std::stof(elem->Attribute("max") ?: "255");
            vTer.push_back(
                Object(elem->Attribute("file"), parse_colour(elem, colour)));

            gb.insert_terrain(elem->Attribute("file"), min_height, max_height);
        } else {
            vGroup.push_back(Parser(elem, parse_colour(elem, colour), gb));
        }
    }
    return Group(
        std::move(vTran),
        std::move(vMod),
        std::move(vTer),
        colour,
        std::move(vGroup));
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

void Group::draw_group(float elapsed, bool DEBUG, GroupBuffer const& group_buffer) const {
    for (auto const& t : transformations) t.apply(DEBUG, elapsed);
    for (auto const& m : models) {
        m.apply_colour();
        group_buffer.draw_model(m.model_name());
    }
    for (auto const& t : terrains) {
        t.apply_colour();
        group_buffer.draw_terrain(t.model_name());
    }
    for (auto const& g : subgroups) {
        glPushMatrix();
        g.draw_group(elapsed, DEBUG, group_buffer);
        glPopMatrix();
    }
}
