// Copyright 2026 Fern Aerell.
// SPDX-License-Identifier: Apache-2.0

#include "aerell/semantic.hpp"

namespace aerell
{

Semantic::Semantic(Parser& parser) : parser(parser) {}

AST Semantic::analyze() { return this->parser.parse(); }

} // namespace aerell