// Copyright 2026 Fern Aerell.
// SPDX-License-Identifier: Apache-2.0

#include <cassert>

#include "aerell/module/inst.hpp"

namespace aerell
{

void Inst::addInst(InstKind kind, std::string_view operand)
{
    this->kinds.push_back(kind);
    this->operands.push_back(operand);
}

std::string_view Inst::getOperand(std::uint32_t index) const
{
    assert(index < this->operands.size());
    return this->operands[index];
}

InstKind Inst::getKind(std::uint32_t index) const
{
    assert(index < this->kinds.size());
    return this->kinds[index];
}

std::uint32_t Inst::size() const { return this->kinds.size(); }

} // namespace aerell