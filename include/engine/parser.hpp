#ifndef PARSER_H
#define PARSER_H
#include "engine/group.hpp"
#include "engine/model.hpp"
#include "utils/types.hpp"

#include <optional>
#include <unordered_map>
#include <vector>

namespace Parser {

auto parse_group(const char* fileName, GroupBuffer& gb) -> Group;

}

#endif // PARSER_H
