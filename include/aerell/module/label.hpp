// Copyright 2026 Fern Aerell.
// SPDX-License-Identifier: Apache-2.0

#pragma once

#include "aerell/module/inst.hpp"

namespace aerell
{

class Label
{
  public:
    Inst& getInst();
    const Inst& getInst() const;
    void setPub(bool val);

  private:
    Inst inst;
    bool pub = false;
};

} // namespace aerell