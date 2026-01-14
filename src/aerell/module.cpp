// Copyright 2026 Fern Aerell.
// SPDX-License-Identifier: Apache-2.0

#include "aerell/module.hpp"

namespace aerell
{

Module::Module() : name("unknown") {}

void Module::setName(std::string_view name) { this->name = name; }

std::string Module::toStr() const { return "# name: " + this->name; }

} // namespace aerell