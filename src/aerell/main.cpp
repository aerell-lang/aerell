// Copyright 2026 Fern Aerell.
// SPDX-License-Identifier: Apache-2.0

#include <print>

#include "aerell/file.hpp"
#include "aerell/lexer.hpp"
#include "aerell/parser.hpp"
#include "aerell/semantic.hpp"
#include "aerell/ir.hpp"
#include "aerell/module.hpp"

int main()
{
    aerell::File file{"main.arl"};
    aerell::Lexer lexer{file};
    aerell::Parser parser{lexer};
    aerell::Semantic semantic{parser};
    aerell::IR ir{semantic};
    aerell::Module module = ir.gen();
    std::println("{}", module.toStr());

    return 0;
}