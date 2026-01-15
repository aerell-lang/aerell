// Copyright 2026 Fern Aerell.
// SPDX-License-Identifier: Apache-2.0

#include "aerell/vm.hpp"
#include <print>

namespace aerell
{

VM::VM(Module& module) : module(module) {}

int VM::run()
{
    const auto& inst = this->module.getLabel("init").getInst();
    for(std::uint32_t i = 0; i < inst.size(); i++)
    {
        switch(inst.getKind(i))
        {
        case InstKind::PRINT: std::println("{}", inst.getOperand(i)); break;
        }
    }
    return 0;
}

} // namespace aerell