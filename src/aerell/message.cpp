// Copyright 2026 Fern Aerell.
// SPDX-License-Identifier: Apache-2.0

#include <algorithm>
#include <format>
#include <map>
#include <print>
#include <utility>

#include "aerell/message.hpp"

namespace aerell
{

struct LexStrPointData
{
    const Lexeme* lex = nullptr;
    const std::string_view* str = nullptr;
    bool pointer = false;
};

void Message::print(const File& file, const std::vector<Lexeme>& lexemes, const std::vector<std::string_view>& messages)
{
    if(lexemes.empty()) return;

    // Calculate the maximum width of the row number for alignment.
    uint32_t maxLine = 0;
    for(const auto& lex : lexemes) maxLine = std::max(maxLine, file.toLocation(lex).line);

    int indentWidth = std::to_string(maxLine).length();

    // Helper for spaces
    auto getPadding = [](int n) { return n > 0 ? std::string(n, ' ') : ""; };

    // Prefix for unnumbered lines (follows the indentWidth width)
    // Format: " |" (spaces equal to indentWidth + 1 space + '|')
    auto printPrefix = [&]() { std::print("{} | ", getPadding(indentWidth)); };

    // Header: File Path (Starts with a space to align with '|')
    auto loc = file.toLocation(lexemes.front());
    std::print("{}---> {}:{}:{}\n", getPadding(indentWidth), file.getPath().generic_string(), loc.line, loc.column);
    printPrefix();
    std::print("\n");

    // 3. Grouping & Sorting
    std::map<uint32_t, std::vector<LexStrPointData>> grouped;
    for(size_t i = 0; i < lexemes.size(); i++)
    {
        const Lexeme* lexeme = &lexemes[i];
        const std::string_view* message = nullptr;
        if(i < messages.size()) message = &messages[i];
        grouped[file.toLocation(lexemes[i]).line].push_back({lexeme, message, (i == 0)});
    }

    for(auto& [lineNum, lineLexAndMsgs] : grouped)
    {
        std::sort(
            lineLexAndMsgs.begin(), lineLexAndMsgs.end(), [&](const LexStrPointData& a, const LexStrPointData& b) {
                return file.toLocation(*a.lex).column < file.toLocation(*b.lex).column;
            });

        // Print lines of code: Line numbers are right-aligned with {:>{}}
        std::print("{:>{}} | {}\n", lineNum, indentWidth, file.getLineText(lineNum));

        // Main pointer line
        printPrefix();
        int currentPos = 1;
        for(const auto& lineLexAndMsg : lineLexAndMsgs)
        {
            const Lexeme* lex = lineLexAndMsg.lex;
            auto loc = file.toLocation(*lex);
            std::print(
                "{}{}", getPadding(loc.column - currentPos), std::string(lex->size, lineLexAndMsg.pointer ? '^' : '-'));
            currentPos = loc.column + lex->size;
        }
        if(lineLexAndMsgs.back().str != nullptr) std::print(" {}\n", *lineLexAndMsgs.back().str);

        // Multiline label line
        if(lineLexAndMsgs.size() > 1)
        {
            for(int i = lineLexAndMsgs.size() - 2; i >= 0; --i)
            {
                // Vertical Pipe Row
                printPrefix();
                currentPos = 1;
                for(int j = 0; j <= i; ++j)
                {
                    auto loc = file.toLocation(*lineLexAndMsgs[j].lex);
                    std::print("{}|", getPadding(loc.column - currentPos));
                    currentPos = loc.column + 1;
                }
                std::print("\n");

                // Label Text Line
                printPrefix();
                currentPos = 1;
                for(int j = 0; j < i; ++j)
                {
                    auto loc = file.toLocation(*lineLexAndMsgs[j].lex);
                    std::print("{}|", getPadding(loc.column - currentPos));
                    currentPos = loc.column + 1;
                }
                auto locLabel = file.toLocation(*lineLexAndMsgs[i].lex);
                if(lineLexAndMsgs[i].str != nullptr)
                    std::print("{}{}\n", getPadding(locLabel.column - currentPos), *lineLexAndMsgs[i].str);
            }
        }
        printPrefix();
        std::print("\n");
    }
}

void Message::print(const File& file, const Lexeme& lexeme, std::string_view message)
{
    print(file, std::vector<Lexeme>{lexeme}, {message});
}

void Message::print(const File& file, ErrorCode ec, const std::vector<Lexeme>& lexemes)
{
    std::print("\nerror[{}]: ", toStr(ec));
    std::vector<std::string_view> messages;
    switch(ec)
    {
    case ErrorCode::E0:
        std::println("unsupported character");
        messages.push_back("help: this character isn't supported here, please delete it");
        break;
    case ErrorCode::E1:
        std::println("integer literal is too large");
        messages.push_back(
            "help: the minimum integer literal is -9223372036854775808 and the maximum is 18446744073709551615");
        break;
    }
    print(file, lexemes, messages);
}

void Message::print(const File& file, ErrorCode ec, const Lexeme& lexeme)
{
    print(file, ec, std::vector<Lexeme>{lexeme});
}

} // namespace aerell