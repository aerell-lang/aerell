// Copyright 2026 Fern Aerell.
// SPDX-License-Identifier: Apache-2.0

#include <cassert>
#include <format>
#include <utility>
#include <vector>

#include "aerell/ast/ast.hpp"

namespace aerell
{

AST::AST(const File& file) : file(file)
{
    this->addKind(ASTKind::NONE);
    this->addData1(0);
    this->addData2(0);
}

const File& AST::getFile() const { return this->file; }

std::string AST::toStr() const
{
    std::string str;
    std::vector<std::pair<std::uint32_t, std::uint32_t>> indexs{{1, 0}};
    while(!indexs.empty())
    {
        auto [index, indent] = indexs.back();
        indexs.pop_back();

        std::string indentStr(indent, ' ');

        auto kind = this->getKind(index);
        switch(kind)
        {
        case ASTKind::NONE: break;
        case ASTKind::ROOT:
        case ASTKind::STMT:
        case ASTKind::EXPR:
            str += std::format("{}[{}]\n", indentStr, aerell::toStr(kind));
            indexs.push_back({this->getData2(index), indent});
            indexs.push_back({this->getData1(index), indent + 1});
            break;
        case ASTKind::INTL:
            str += std::format(
                "{}[{}] {}\n", indentStr, aerell::toStr(kind),
                this->file.getLexemeText({this->getData1(index), this->getData2(index)}));
            break;
        }
    }
    return str;
}

std::uint32_t AST::addIntl(std::uint32_t offset, std::uint32_t size)
{
    this->addData1(offset);
    this->addData2(size);
    return this->addKind(ASTKind::INTL);
}

std::uint32_t AST::addData1(std::uint32_t data)
{
    std::uint32_t index = static_cast<std::uint32_t>(this->data1.size());
    this->data1.push_back(data);
    return index;
}

std::uint32_t AST::addData2(std::uint32_t data)
{
    std::uint32_t index = static_cast<std::uint32_t>(this->data2.size());
    this->data2.push_back(data);
    return index;
}

std::uint32_t AST::addKind(ASTKind kind)
{
    std::uint32_t index = static_cast<std::uint32_t>(this->kinds.size());
    this->kinds.push_back(kind);
    return index;
}

void AST::setKind(std::uint32_t index, ASTKind kind)
{
    assert(index < this->kinds.size());
    this->kinds[index] = kind;
}

void AST::setData1(std::uint32_t index, std::uint32_t data)
{
    assert(index < this->data1.size());
    this->data1[index] = data;
}

void AST::setData2(std::uint32_t index, std::uint32_t data)
{
    assert(index < this->data2.size());
    this->data2[index] = data;
}

ASTKind AST::getKind(std::uint32_t index) const
{
    assert(index < this->kinds.size());
    return this->kinds[index];
}

std::uint32_t AST::getData1(std::uint32_t index) const
{
    assert(index < this->data1.size());
    return this->data1[index];
}

std::uint32_t AST::getData2(std::uint32_t index) const
{
    assert(index < this->data2.size());
    return this->data2[index];
}

} // namespace aerell