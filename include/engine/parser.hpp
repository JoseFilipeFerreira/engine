#ifndef PARSER_H
#define PARSER_H
#include "engine/model.hpp"
#include "engine/group.hpp"
#include "utils/types.hpp"

#include <optional>
#include <unordered_map>
#include <vector>

auto Parser(const char* fileName, GroupBuffer& gb) -> Group;

#endif // PARSER_H
