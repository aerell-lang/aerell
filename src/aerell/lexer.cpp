// Copyright 2026 Fern Aerell.
// SPDX-License-Identifier: Apache-2.0

#include "aerell/lexer.hpp"

namespace aerell
{

Lexer::Lexer(File& file) : file(file), token(file) {}

const Token& Lexer::getToken() { return this->token; }

} // namespace aerell