// Copyright 2026 Fern Aerell.
// SPDX-License-Identifier: Apache-2.0

#pragma once

#include <vector>

#include "aerell/module/inst_kind.hpp"

namespace aerell
{

class Inst
{
  public:
    void addInst(InstKind kind, std::string_view operand);

    std::string_view getOperand(std::uint32_t index) const;
    InstKind getKind(std::uint32_t index) const;

    std::uint32_t size() const;

  private:
    std::vector<std::string_view> operands;
    std::vector<InstKind> kinds;
};

} // namespace aerell