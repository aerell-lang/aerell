// Copyright 2026 Fern Aerell.
// SPDX-License-Identifier: Apache-2.0

#pragma once

#include <vector>

#include "aerell/file.hpp"
#include "aerell/ast/ast_kind.hpp"

namespace aerell
{

class AST
{
  public:
    AST(const File& file);

    const File& getFile() const;

    std::string toStr() const;

    std::uint32_t addIntl(std::uint32_t offset, std::uint32_t size);

    std::uint32_t addKind(ASTKind kind);
    std::uint32_t addData1(std::uint32_t data);
    std::uint32_t addData2(std::uint32_t data);

    void setKind(std::uint32_t index, ASTKind kind);
    void setData1(std::uint32_t index, std::uint32_t data);
    void setData2(std::uint32_t index, std::uint32_t data);

    ASTKind getKind(std::uint32_t index) const;
    std::uint32_t getData1(std::uint32_t index) const;
    std::uint32_t getData2(std::uint32_t index) const;

    void reset();
    bool empty() const;

  private:
    const File& file;
    std::vector<ASTKind> kinds;
    std::vector<std::uint32_t> data1; // index | offset
    std::vector<std::uint32_t> data2; // index | size
};

} // namespace aerell