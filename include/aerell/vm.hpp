// Copyright 2026 Fern Aerell.
// SPDX-License-Identifier: Apache-2.0

#pragma once

#include "aerell/module.hpp"

namespace aerell
{

class VM
{
  public:
    VM(Module& module);
    int run();

  private:
    Module& module;
};

} // namespace aerell