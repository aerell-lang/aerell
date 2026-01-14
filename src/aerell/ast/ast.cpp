// Copyright 2026 Fern Aerell.
// SPDX-License-Identifier: Apache-2.0

#include <cassert>
#include <format>

#include "aerell/ast/ast.hpp"

namespace aerell
{

AST::AST(const File& file) : file(file)
{
    this->addKind(ASTKind::NONE);
    this->addData1(0);
    this->addData2(0);
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
    std::uint32_t kindIndex = static_cast<std::uint32_t>(this->kinds.size());
    this->kinds.push_back(kind);
    return kindIndex;
}

std::uint32_t AST::addIntl(std::uint32_t offset, std::uint32_t size)
{
    this->addData1(offset);
    this->addData2(size);
    return this->addKind(ASTKind::INTL);
}

std::string AST::toStr() const
{
    std::string str;
    for(std::size_t i = 0; i < this->kinds.size(); i++)
    {
        ASTKind kind = this->kinds.at(i);
        switch(kind)
        {
        case ASTKind::NONE: break;
        case ASTKind::INTL:
            std::size_t offset = this->data1[i];
            std::size_t size = this->data2[i];
            str += std::format(
                "[{}] offset: {}, size: {}, lexeme: {:.{}}", aerell::toStr(kind), offset, size,
                this->file.getData() + offset, size);
            break;
        }
    }
    return str;
}

const File& AST::getFile() const { return this->file; }

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
    return this->data1[index];
}

} // namespace aerell