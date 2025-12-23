/*
 * This file is part of The Aerell Programming Language
 *
 * Copyright 2025 The Aerell Programming Language Authors
 * Licensed under the Apache License, Version 2.0.
 * See the LICENSE file for details.
 */

#pragma once

#include "llvm/IR/Module.h"

namespace Aerell
{

class CodeGen
{
  public:
    static bool obj(const char* name, const std::unique_ptr<llvm::Module>& module);
};

} // namespace Aerell