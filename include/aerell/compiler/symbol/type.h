#pragma once

#include <array>

namespace Aerell
{

enum Type
{
    VOID,
    I32,
    STR
};

constexpr std::array<std::pair<Type, const char*>, 3> TypeNames{{
    {Type::VOID, "VOID"},
    {Type::I32, "I32"},
    {Type::STR, "STR"},
}};

constexpr const char* to_string(Type t)
{
    for(auto& [k, v] : TypeNames)
        if(k == t) return v;
    return "UNKNOWN";
}

} // namespace Aerell