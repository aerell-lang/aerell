// Copyright 2026 Fern Aerell.
// SPDX-License-Identifier: Apache-2.0

#include "aerell/file.hpp"
#include "aerell/lexer.hpp"
#include "aerell/parser.hpp"
#include "aerell/semantic.hpp"

int main()
{
    aerell::File file{"main.arl"};
    aerell::Lexer lexer{file};
    aerell::Parser parser{lexer};
    aerell::Semantic semantic{parser};
    semantic.analyze();

    return 0;
}