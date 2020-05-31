#include "engine/parser.hpp"

#include "deps/tinyxml.hpp"
#include "engine/light.hpp"
#include "utils/colour.hpp"
#include "utils/point.hpp"
#include "utils/types.hpp"

#include <functional>
#include <stdexcept>
#include <tuple>

namespace Parser {

using ParseState =
    std::tuple<const char*, TiXmlElement const*, Colour, GroupBuffer*>;

auto parse_group_colour(const char* file_name, GroupBuffer* gb, Colour c)
    -> Group;

[[noreturn]] void
throw_fancy_error(ParseState ps, std::string_view const& error) {
    std::stringstream ss;
    auto& [file_name, elem, colour, gb] = ps;
    ss << "Error while parsing: " << file_name << '\n'
       << elem->Row() << ":" << elem->Column() << ": " << RED
       << "error: " << RESET << error;
    throw std::invalid_argument(ss.str());
}

[[noreturn]] void throw_fancy_error(
    ParseState ps,
    std::string_view const& error,
    std::string_view const& attribute) {
    auto& [file_name, elem, colour, gb] = ps;

    std::stringstream ss;
    ss << "Error while parsing file: " << file_name << '\n'
       << elem->Row() << ":" << elem->Column() << ": " << RED
       << "error: " << RESET << error << " in Attribute: \"" << attribute
       << "\"";

    throw std::invalid_argument(ss.str());
}

auto parse_float(ParseState ps, std::string const& attribute, float def)
    -> float {
    auto& [_, elem, __, ___] = ps;
    auto res = elem->Attribute(attribute);
    if (res != nullptr) {
        try {
            return std::stof(*res);
        } catch (std::exception const& e) {
            throw_fancy_error(ps, "Invalid float value", attribute);
        }
    }

    return def;
}

auto parse_colour(ParseState ps, std::string const& attribute)
    -> std::optional<Colour> {
    auto& [file_name, elem, colour, gb] = ps;
    //#RRGGBB or #RRGGBBAA
    if (elem->Attribute(attribute)) {
        u32 r, g, b, a;
        std::string const& s = *elem->Attribute(attribute);
        switch (s.length()) {
            case 7:
                if (std::sscanf(s.data(), "#%02x%02x%02x", &r, &g, &b) != 3)
                    throw_fancy_error(ps, "Invalid colour format", attribute);
                a = 255;
                break;
            case 9:
                if (std::sscanf(
                        s.data(), "#%02x%02x%02x%02x", &r, &g, &b, &a) != 4)
                    throw_fancy_error(ps, "Invalid colour format", attribute);
                break;
            default:
                throw_fancy_error(ps, "Invalid colour format", attribute);
        }
        return Colour(r / 255.0f, g / 255.0f, b / 255.0f, a / 255.0f);
    }

    // prefixR="1" prefixG="1" prefixB="1" prefixA="1"
    if (elem->Attribute(attribute + "R") || elem->Attribute(attribute + "G") ||
        elem->Attribute(attribute + "B") || elem->Attribute(attribute + "A")) {
        float r = parse_float(ps, attribute + "R", 0);
        float g = parse_float(ps, attribute + "G", 0);
        float b = parse_float(ps, attribute + "B", 0);
        float a = parse_float(ps, attribute + "A", 1);

        if (r > 1 || r < 0)
            throw_fancy_error(
                ps, "Colour value must be between 0 and 1", attribute + "R");
        if (g > 1 || g < 0)
            throw_fancy_error(
                ps, "Colour value must be between 0 and 1", attribute + "G");
        if (b > 1 || b < 0)
            throw_fancy_error(
                ps, "Colour value must be between 0 and 1", attribute + "B");
        if (a > 1 || a < 0)
            throw_fancy_error(
                ps, "Colour value must be between 0 and 1", attribute + "A");

        return Colour(r, g, b, a);
    }

    return std::nullopt;
}

auto parse_point(ParseState ps, std::string const& prefix, float def) -> Point {
    float x = parse_float(ps, prefix + "X", def);
    float y = parse_float(ps, prefix + "Y", def);
    float z = parse_float(ps, prefix + "Z", def);

    return Point(x, y, z);
}

auto parse_vector(ParseState ps, std::string const& prefix, float def)
    -> Vector {
    return Vector(Point(0, 0, 0), parse_point(ps, prefix, def));
}

auto update_colour(ParseState ps, Colour def) -> Colour {
    return parse_colour(ps, "colour").value_or(def);
}

auto parse_points(ParseState ps) -> std::vector<Point> {
    auto& [_, root, __, ___] = ps;
    std::vector<Point> points;
    for (auto elem = root->FirstChildElement(); elem != NULL;
         elem = elem->NextSiblingElement()) {
        std::string_view type = elem->Value();
        if (type == "point") {
            points.push_back(parse_point({_, elem, __, ___}, "", 0));
        }
    }
    if (points.size() < 4)
        throw_fancy_error(
            ps, "There need to be at least 4 points in a Catmull-Rom curve");
    return points;
}

template<typename T>
auto parse_object(ParseState ps, BoundingBox bb) -> Object<T> {
    auto& [_, elem, colour, gb] = ps;

    std::optional<std::string> tex;
    if (elem->Attribute("texture")) {
        tex = std::make_optional(elem->Attribute("texture"));
        gb->insert_texture(tex.value());
    }

    auto diffuse = parse_colour(ps, "diff").value_or(Colour());
    auto specular = parse_colour(ps, "spec").value_or(Colour(0, 0, 0, 0));
    auto emissive = parse_colour(ps, "emis").value_or(Colour(0, 0, 0, 0));
    auto ambient = parse_colour(ps, "ambi").value_or(Colour(0, 0, 0, 0));

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

auto parse_light(ParseState ps) -> Light {
    auto& [_, elem, __, ___] = ps;
    std::string_view const type = elem->Attribute("type");

    auto colour = parse_colour(ps, "colour").value_or(Colour());

    if (type == "POINT") {
        return PointLight(parse_point(ps, "pos", 0), colour);

    } else if (type == "DIRECTIONAL") {
        return DirectionalLight(parse_vector(ps, "dir", 0), colour);

    } else if (type == "SPOT") {
        return SpotLight(
            parse_point(ps, "pos", 0), parse_vector(ps, "dir", 0), colour);
    }

    throw_fancy_error(ps, "Invalid light type", "type");
}

auto recursive_parse(ParseState ps) -> Group {
    auto& [file_name, root, colour, gb] = ps;

    std::vector<Transform> vTran;
    std::vector<Object<model_t>> vMod;
    std::vector<Object<terrain_t>> vTer;
    std::vector<Group> vGroup;
    std::vector<Light> vLight;

    for (auto const* elem = root->FirstChildElement(); elem != nullptr;
         elem = elem->NextSiblingElement()) {
        auto new_colour = update_colour(ps, colour);
        ParseState new_state = {file_name, elem, new_colour, gb};
        std::string_view type = elem->Value();

        if (type == "translate") {
            if (elem->Attribute("time")) {
                float time = parse_float(new_state, "time", 0);
                auto point_vec = parse_points(new_state);
                vTran.push_back(CatmullRon(time, std::move(point_vec)));
            } else {
                vTran.push_back(Translate(parse_vector(new_state, "", 0)));
            }

        } else if (type == "rotate") {
            float ang = parse_float(new_state, "angle", 0);
            float time = parse_float(new_state, "time", 0);
            vTran.push_back(
                Rotate(ang, parse_vector(new_state, "axis", 0), time));

        } else if (type == "scale") {
            vTran.push_back(Scale(parse_vector(new_state, "", 1)));

        } else if (type == "light") {
            vLight.push_back(parse_light(new_state));

        } else if (type == "model") {
            auto bb = gb->insert_model(elem->Attribute("file"));

            auto m_obj = parse_object<model_t>(new_state, bb);
            vMod.push_back(m_obj);

        } else if (type == "terrain") {
            float min_height = parse_float(new_state, "min", 0);
            float max_height = parse_float(new_state, "max", 255);
            auto bb = gb->insert_terrain(
                elem->Attribute("file"), min_height, max_height);

            auto t_obj = parse_object<terrain_t>(new_state, bb);
            vTer.push_back(t_obj);
        } else if (type == "include") {
            vGroup.push_back(
                parse_group_colour(elem->Attribute("file"), gb, new_colour));

        } else {
            vGroup.push_back(recursive_parse(new_state));
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

auto parse_group_colour(const char* file_name, GroupBuffer* gb, Colour c)
    -> Group {
    TiXmlDocument doc(file_name);
    if (!doc.LoadFile()) {
        throw doc.ErrorDesc();
    }

    TiXmlElement* root = doc.FirstChildElement();
    if (!root) {
        throw "Failed to load file: No root element.";
    }

    ParseState parse_state =
        std::make_tuple(file_name, doc.FirstChildElement(), c, gb);
    return recursive_parse(parse_state);
}

auto parse_group(const char* file_name, GroupBuffer& gb) -> Group {
    return parse_group_colour(file_name, &gb, Colour());
}

} // namespace Parser
