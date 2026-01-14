// Copyright 2026 Fern Aerell.
// SPDX-License-Identifier: Apache-2.0

#pragma once

#include <string>

namespace aerell
{

class Module
{
  public:
    Module();

    void setName(std::string_view name);

    std::string toStr() const;

  private:
    std::string name;
};

} // namespace aerell