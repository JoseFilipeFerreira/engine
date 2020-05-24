#include "engine/parser.hpp"

#include "deps/tinyxml.hpp"
#include "engine/light.hpp"
#include "utils/colour.hpp"
#include "utils/point.hpp"
#include "utils/types.hpp"

#include <stdexcept>

namespace Parser {

[[noreturn]] void
throw_fancy_error(TiXmlElement const* elem, std::string_view const& error) {
    std::stringstream ss;
    ss << elem->Row() << ":" << elem->Column() << ": " << RED
       << "error: " << RESET << error;
    throw std::invalid_argument(ss.str());
}

[[noreturn]] void throw_fancy_error(
    TiXmlElement const* elem,
    std::string_view const& error,
    std::string_view const& attribute) {
    std::stringstream ss;
    ss << elem->Row() << ":" << elem->Column() << ": " << RED
       << "error: " << RESET << error << " in Attribute: \"" << attribute
       << "\"";

    throw std::invalid_argument(ss.str());
}

auto parse_float(
    TiXmlElement const* elem, std::string const& attribute, float def)
    -> float {
    auto res = elem->Attribute(attribute);
    if (res != nullptr) {
        try {
            return std::stof(*res);
        } catch (std::exception const& e) {
            throw_fancy_error(elem, "Invalid float value", attribute);
        }
    }

    return def;
}

auto parse_colour(TiXmlElement const* elem, std::string const& attribute)
    -> std::optional<Colour> {
    //#RRGGBB or #RRGGBBAA
    if (elem->Attribute(attribute)) {
        u32 r, g, b, a;
        std::string const& s = *elem->Attribute(attribute);
        switch (s.length()) {
            case 7:
                if (std::sscanf(s.data(), "#%02x%02x%02x", &r, &g, &b) != 3)
                    throw_fancy_error(elem, "Invalid colour format", attribute);
                a = 255;
                break;
            case 9:
                if (std::sscanf(
                        s.data(), "#%02x%02x%02x%02x", &r, &g, &b, &a) != 4)
                    throw_fancy_error(elem, "Invalid colour format", attribute);
                break;
            default:
                throw_fancy_error(elem, "Invalid colour format", attribute);
        }
        return Colour(r / 255.0f, g / 255.0f, b / 255.0f, a / 255.0f);
    }

    // prefixR="1" prefixG="1" prefixB="1" prefixA="1"
    if (elem->Attribute(attribute + "R") || elem->Attribute(attribute + "G") ||
        elem->Attribute(attribute + "B") || elem->Attribute(attribute + "A")) {
        float r = parse_float(elem, attribute + "R", 1);
        float g = parse_float(elem, attribute + "G", 1);
        float b = parse_float(elem, attribute + "B", 1);
        float a = parse_float(elem, attribute + "A", 1);

        if (r > 1 || r < 0)
            throw_fancy_error(
                elem, "Colour value must be between 0 and 1", attribute + "R");
        if (g > 1 || g < 0)
            throw_fancy_error(
                elem, "Colour value must be between 0 and 1", attribute + "G");
        if (b > 1 || b < 0)
            throw_fancy_error(
                elem, "Colour value must be between 0 and 1", attribute + "B");
        if (a > 1 || a < 0)
            throw_fancy_error(
                elem, "Colour value must be between 0 and 1", attribute + "A");

        return Colour(r, g, b, a);
    }

    return std::nullopt;
}

auto parse_point(TiXmlElement const* elem, std::string const& prefix, float def)
    -> Point {
    float x = parse_float(elem, prefix + "X", def);
    float y = parse_float(elem, prefix + "Y", def);
    float z = parse_float(elem, prefix + "Z", def);

    return Point(x, y, z);
}

auto parse_vector(
    TiXmlElement const* elem, std::string const& prefix, float def) -> Vector {
    return Vector(Point(0, 0, 0), parse_point(elem, prefix, def));
}

auto update_colour(TiXmlElement const* elem, Colour colour) -> Colour {
    return parse_colour(elem, "colour").value_or(colour);
}

auto parse_points(TiXmlElement const* root) -> std::vector<Point> {
    std::vector<Point> points;
    for (auto elem = root->FirstChildElement(); elem != NULL;
         elem = elem->NextSiblingElement()) {
        std::string_view type = elem->Value();
        if (type == "point") {
            points.push_back(parse_point(elem, "", 0));
        }
    }
    if (points.size() < 4)
        throw_fancy_error(
            root, "There need to be at least 4 points in a Catmull-Rom curve");
    return points;
}

template<typename T>
auto parse_object(
    TiXmlElement const* elem, Colour colour, GroupBuffer& gb, BoundingBox bb)
    -> Object<T> {
    std::optional<std::string> tex;
    if (elem->Attribute("texture")) {
        tex = std::make_optional(elem->Attribute("texture"));
        gb.insert_texture(tex.value());
    }

    auto diffuse = parse_colour(elem, "diff").value_or(Colour());
    auto specular = parse_colour(elem, "spec").value_or(Colour(0, 0, 0, 0));
    auto emissive = parse_colour(elem, "emis").value_or(Colour(0, 0, 0, 0));
    auto ambient = parse_colour(elem, "ambi").value_or(Colour(0, 0, 0, 0));

    return Object<T>(
        elem->Attribute("file"),
        tex,
        colour,
        diffuse,
        specular,
        emissive,
        ambient,
        bb);
}

auto parse_light(TiXmlElement const* elem) -> Light {
    std::string_view const type = elem->Attribute("type");

    auto colour = parse_colour(elem, "colour").value_or(Colour());

    if (type == "POINT") {
        return PointLight(parse_point(elem, "pos", 0), colour);

    } else if (type == "DIRECTIONAL") {
        return DirectionalLight(parse_vector(elem, "dir", 0), colour);

    } else if (type == "SPOT") {
        return SpotLight(
            parse_point(elem, "pos", 0), parse_vector(elem, "dir", 0), colour);
    }

    throw_fancy_error(elem, "Invalid light type", "type");
}

auto recursive_parse(TiXmlElement const* root, Colour colour, GroupBuffer& gb)
    -> Group {
    std::vector<Transform> vTran;
    std::vector<Object<model_t>> vMod;
    std::vector<Object<terrain_t>> vTer;
    std::vector<Group> vGroup;
    std::vector<Light> vLight;

    for (auto const* elem = root->FirstChildElement(); elem != nullptr;
         elem = elem->NextSiblingElement()) {
        std::string_view type = elem->Value();

        if (type == "translate") {
            if (elem->Attribute("time")) {
                float time = parse_float(elem, "time", 0);
                auto point_vec = parse_points(elem);
                vTran.push_back(CatmullRon(time, std::move(point_vec)));
            } else {
                vTran.push_back(Translate(parse_vector(elem, "", 0)));
            }

        } else if (type == "rotate") {
            float ang = parse_float(elem, "angle", 0);
            float time = parse_float(elem, "time", 0);
            vTran.push_back(Rotate(ang, parse_vector(elem, "axis", 0), time));

        } else if (type == "scale") {
            vTran.push_back(Scale(parse_vector(elem, "", 1)));

        } else if (type == "light") {
            vLight.push_back(parse_light(elem));

        } else if (type == "model") {
            auto bb = gb.insert_model(elem->Attribute("file"));

            auto m_obj = parse_object<model_t>(
                elem, update_colour(elem, colour), gb, bb);
            vMod.push_back(m_obj);

        } else if (type == "terrain") {
            float min_height = parse_float(elem, "min", 0);
            float max_height = parse_float(elem, "max", 255);
            auto bb = gb.insert_terrain(
                elem->Attribute("file"), min_height, max_height);

            auto t_obj = parse_object<terrain_t>(
                elem, update_colour(elem, colour), gb, bb);
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
        std::move(vLight),
        colour,
        std::move(vGroup));
}

auto parse_group(const char* file_name, GroupBuffer& gb) -> Group {
    TiXmlDocument doc(file_name);
    if (!doc.LoadFile()) {
        throw doc.ErrorDesc();
    }

    TiXmlElement* root = doc.FirstChildElement();
    if (!root) {
        throw "Failed to load file: No root element.";
    }

    return recursive_parse(doc.FirstChildElement(), Colour(), gb);
}

} // namespace Parser
