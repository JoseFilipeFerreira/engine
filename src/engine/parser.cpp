#include "engine/parser.hpp"

#include "deps/tinyxml.hpp"
#include "utils/colour.hpp"
#include "utils/point.hpp"
#include "utils/types.hpp"

#include <stdexcept>

void throw_fancy_error(TiXmlElement* elem, std::string const& error) {
    std::stringstream ss;
    ss << elem->Row() << ":" << elem->Column() << ": " << RED
       << "error: " << RESET << error;
    throw std::invalid_argument(ss.str());
}
void throw_fancy_error(
    TiXmlElement* elem,
    std::string const& error,
    std::string const& attribute) {
    std::stringstream ss;
    ss << elem->Row() << ":" << elem->Column() << ": " << RED
       << "error: " << RESET << error << " in Attribute: \"" << attribute
       << "\"";

    throw std::invalid_argument(ss.str());
}

auto parse_colour(TiXmlElement* elem, std::string const& attribute)
    -> std::optional<Colour> {
    if (elem->Attribute(attribute)) {
        u32 r, g, b, a;
        std::string const s = *elem->Attribute(attribute);
        if (s.length() == 7) {
            std::sscanf(s.data(), "#%02x%02x%02x", &r, &g, &b);
            a = 255;

        } else if (s.length() == 9) {
            std::sscanf(s.data(), "#%02x%02x%02x%02x", &r, &g, &b, &a);

        } else {
            throw_fancy_error(elem, "Invalid colour format", attribute);
        }
        return Colour(r / 255.0f, g / 255.0f, b / 255.0f, a / 255.0f);
    }
    return std::nullopt;
}

auto update_colour(TiXmlElement* elem, Colour colour) -> Colour {
    return parse_colour(elem, "colour").value_or(colour);
}

auto parse_points(TiXmlElement* root) -> std::vector<Point> {
    std::vector<Point> points;
    for (auto elem = root->FirstChildElement(); elem != NULL;
         elem = elem->NextSiblingElement()) {
        std::string_view type = elem->Value();
        if (type == "point") {
            float x = std::stof(elem->Attribute("X") ?: "0");
            float y = std::stof(elem->Attribute("Y") ?: "0");
            float z = std::stof(elem->Attribute("Z") ?: "0");
            points.emplace_back(x, y, z);
        }
    }
    if (points.size() < 4)
        throw_fancy_error(
            root, "There need to be at least 4 points in a Catmull-Rom curve");
    return points;
}

Object parse_object(TiXmlElement* elem, Colour colour, GroupBuffer& gb) {
    std::optional<std::string> tex;
    if (elem->Attribute("texture")) {
        tex = std::make_optional(elem->Attribute("texture"));
        gb.insert_texture(tex.value());
    }

    auto diffuse = parse_colour(elem, "diff");
    auto specular = parse_colour(elem, "spec");
    auto emissive = parse_colour(elem, "emis");
    auto ambient = parse_colour(elem, "ambi");

    return Object(
        elem->Attribute("file"),
        tex,
        colour,
        diffuse,
        specular,
        emissive,
        ambient);
}

auto recursive_parse(TiXmlElement* root, Colour colour, GroupBuffer& gb)
    -> Group {
    std::vector<Transform> vTran;
    std::vector<Object> vMod;
    std::vector<Object> vTer;
    std::vector<Group> vGroup;

    for (TiXmlElement* elem = root->FirstChildElement(); elem != nullptr;
         elem = elem->NextSiblingElement()) {
        std::string_view type = elem->Value();

        if (type == "translate") {
            if (elem->Attribute("time")) {
                float const time = std::stof(elem->Attribute("time") ?: "0");
                auto point_vec = parse_points(elem);
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
            auto m_obj = parse_object(elem, update_colour(elem, colour), gb);

            gb.insert_model(m_obj.model_name());

            vMod.push_back(m_obj);

        } else if (type == "terrain") {
            auto t_obj = parse_object(elem, update_colour(elem, colour), gb);

            float min_height = std::stof(elem->Attribute("min") ?: "0");
            float max_height = std::stof(elem->Attribute("max") ?: "255");
            gb.insert_terrain(t_obj.model_name(), min_height, max_height);

            vTer.push_back(t_obj);

        } else {
            vGroup.push_back(
                recursive_parse(elem, update_colour(elem, colour), gb));
        }
    }
    return Group(
        std::move(vTran),
        std::move(vMod),
        std::move(vTer),
        colour,
        std::move(vGroup));
}

auto Parser(const char* fileName, GroupBuffer& gb) -> Group {
    TiXmlDocument doc(fileName);
    if (!doc.LoadFile()) {
        throw doc.ErrorDesc();
    }

    TiXmlElement* root = doc.FirstChildElement();
    if (!root) {
        throw "Failed to load file: No root element.";
    }

    return recursive_parse(doc.FirstChildElement(), Colour(), gb);
}
