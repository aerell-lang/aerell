// Copyright 2026 Fern Aerell.
// SPDX-License-Identifier: Apache-2.0

#include "aerell/vm.hpp"

namespace aerell
{

VM::VM(Module& module) : module(module) {}

int VM::run() { return 0; }

} // namespace aerell