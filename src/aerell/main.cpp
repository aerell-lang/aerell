// Copyright 2026 Fern Aerell.
// SPDX-License-Identifier: Apache-2.0

#include <print>

#include "aerell/file.hpp"
#include "aerell/lexer.hpp"
#include "aerell/ast/ast.hpp"
#include "aerell/parser.hpp"
#include "aerell/semantic.hpp"
#include "aerell/ir.hpp"
#include "aerell/module.hpp"
#include "aerell/vm.hpp"

int main()
{
    auto file = aerell::File::load("main.arl");
    if(!file.has_value())
    {
        std::println("Failed to open file.");
        return 1;
    }
    aerell::Lexer lexer{file.value()};
    aerell::Parser parser{lexer};
    aerell::AST ast = parser.parse();
    aerell::Semantic semantic{ast};
    if(!semantic.analyze()) return 1;
    aerell::IR ir{ast};
    aerell::Module module = ir.gen();
    aerell::VM vm{module};
    return vm.run();
}