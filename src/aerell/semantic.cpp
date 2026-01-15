// Copyright 2026 Fern Aerell.
// SPDX-License-Identifier: Apache-2.0

#include <optional>

#include "aerell/semantic.hpp"
#include "aerell/message.hpp"
#include "aerell/type.hpp"

namespace aerell
{

Semantic::Semantic(AST& ast) : ast(ast) {}

bool Semantic::analyze()
{
    this->hasError = false;
    for(std::uint32_t i = 0; i < this->ast.getKindCount(); i++) this->analyze(i);
    return !hasError;
}

bool strLenLessOrEqual(std::string_view str1, std::string_view str2)
{
    if(str1.length() < str2.length()) return true;
    if(str1.length() > str2.length()) return false;
    return str1 <= str2;
}

std::optional<Type> getType(std::string_view str)
{
    if(str.empty()) return std::nullopt;

    bool isNegative = (str[0] == '-');
    str = isNegative ? str.substr(1) : str;

    if(isNegative)
    {
        // Signed
        if(strLenLessOrEqual(str, "128")) return Type::I8;
        if(strLenLessOrEqual(str, "32768")) return Type::I16;
        if(strLenLessOrEqual(str, "2147483648")) return Type::I32;
        if(strLenLessOrEqual(str, "9223372036854775808")) return Type::I64;
    }
    else
    {
        // Signed
        if(strLenLessOrEqual(str, "1")) return Type::I1;
        if(strLenLessOrEqual(str, "127")) return Type::I8;
        if(strLenLessOrEqual(str, "32767")) return Type::I16;
        if(strLenLessOrEqual(str, "2147483647")) return Type::I32;
        if(strLenLessOrEqual(str, "9223372036854775807")) return Type::I64;
        // Unsigned
        if(strLenLessOrEqual(str, "255")) return Type::U8;
        if(strLenLessOrEqual(str, "65535")) return Type::U16;
        if(strLenLessOrEqual(str, "4294967295")) return Type::U32;
        if(strLenLessOrEqual(str, "18446744073709551615")) return Type::U64;
    }

    return std::nullopt;
}

void Semantic::analyze(std::uint32_t index)
{
    switch(ast.getKind(index))
    {
    case ASTKind::NONE: break;
    case ASTKind::INTL:
        // Check int type
        auto offset = ast.getData1(index);
        auto size = ast.getData2(index);
        auto type = getType({ast.getFile().getData() + offset, size});
        if(type.has_value()) break;
        this->hasError = true;
        Message::print(
            ErrorCode::E0, ast.getFile(),
            {
                {"help: the minimum integer literal is -9223372036854775808 and the maximum is "
                 "18446744073709551615",
                 {offset, size},
                 true},
            });
        break;
    }
}

} // namespace aerell