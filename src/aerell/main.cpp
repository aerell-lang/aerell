// Copyright 2026 Fern Aerell.
// SPDX-License-Identifier: Apache-2.0

#include <print>

#include "aerell/file.hpp"
#include "aerell/lexer.hpp"

int main()
{
    aerell::File file("main.arl");
    aerell::Lexer lexer(file);
    std::print("Hello Aerell!\n");
    return 0;
}