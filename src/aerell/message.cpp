// Copyright 2026 Fern Aerell.
// SPDX-License-Identifier: Apache-2.0

#include <algorithm>
#include <format>
#include <map>
#include <print>

#include "aerell/message.hpp"

namespace aerell
{

void Message::print(const File& file, const std::vector<Lexeme>& lexemes)
{
    if(lexemes.empty()) return;

    // Calculate the maximum width of the row number for alignment.
    uint32_t maxLine = 0;
    for(const auto& lex : lexemes) maxLine = std::max(maxLine, file.toLocation(lex.data).line);

    int indentWidth = std::to_string(maxLine).length();

    // Helper for spaces
    auto getPadding = [](int n) { return n > 0 ? std::string(n, ' ') : ""; };

    // Prefix for unnumbered lines (follows the indentWidth width)
    // Format: " |" (spaces equal to indentWidth + 1 space + '|')
    auto printPrefix = [&]() { std::print("{} | ", getPadding(indentWidth)); };

    // Header: File Path (Starts with a space to align with '|')
    for(const auto& lex : lexemes)
    {
        if(lex.pointer)
        {
            auto loc = file.toLocation(lex.data);
            std::print(
                "{}---> {}:{}:{}\n", getPadding(indentWidth), file.getPath().generic_string(), loc.line, loc.column);
            printPrefix();
            std::print("\n");
            break;
        }
    }

    // 3. Grouping & Sorting
    std::map<uint32_t, std::vector<const Lexeme*>> grouped;
    for(const auto& lex : lexemes) grouped[file.toLocation(lex.data).line].push_back(&lex);

    for(auto& [lineNum, lineLexemes] : grouped)
    {
        std::sort(lineLexemes.begin(), lineLexemes.end(), [&](const Lexeme* a, const Lexeme* b) {
            return file.toLocation(a->data).column < file.toLocation(b->data).column;
        });

        // Print lines of code: Line numbers are right-aligned with {:>{}}
        std::print("{:>{}} | {}\n", lineNum, indentWidth, file.getLineText(lineNum));

        // Main pointer line
        printPrefix();
        int currentPos = 1;
        for(const auto* lex : lineLexemes)
        {
            auto loc = file.toLocation(lex->data);
            std::print(
                "{}{}", getPadding(loc.column - currentPos), std::string(lex->data.size, lex->pointer ? '^' : '-'));
            currentPos = loc.column + lex->data.size;
        }
        std::print(" {}\n", lineLexemes.back()->message);

        // Multiline label line
        if(lineLexemes.size() > 1)
        {
            for(int i = lineLexemes.size() - 2; i >= 0; --i)
            {
                // Vertical Pipe Row
                printPrefix();
                currentPos = 1;
                for(int j = 0; j <= i; ++j)
                {
                    auto loc = file.toLocation(lineLexemes[j]->data);
                    std::print("{}|", getPadding(loc.column - currentPos));
                    currentPos = loc.column + 1;
                }
                std::print("\n");

                // Label Text Line
                printPrefix();
                currentPos = 1;
                for(int j = 0; j < i; ++j)
                {
                    auto loc = file.toLocation(lineLexemes[j]->data);
                    std::print("{}|", getPadding(loc.column - currentPos));
                    currentPos = loc.column + 1;
                }
                auto locLabel = file.toLocation(lineLexemes[i]->data);
                std::print("{}{}\n", getPadding(locLabel.column - currentPos), lineLexemes[i]->message);
            }
        }
        printPrefix();
        std::print("\n");
    }
}

void Message::print(const File& file, const Lexeme& lexeme) { print(file, std::vector<Lexeme>{lexeme}); }

void Message::print(ErrorCode code, const File& file, const std::vector<Lexeme>& lexemes)
{
    std::print("error[{}]: ", toStr(code));
    switch(code)
    {
    case ErrorCode::E0: std::println("integer literal is too large"); break;
    }
    print(file, lexemes);
}

void Message::print(ErrorCode code, const File& file, const Lexeme& lexeme)
{
    print(code, file, std::vector<Lexeme>{lexeme});
}

} // namespace aerell