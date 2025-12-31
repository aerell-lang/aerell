/*
 * This file is part of The Aerell Programming Language
 *
 * Copyright 2025 The Aerell Programming Language Authors
 * Licensed under the Apache License, Version 2.0.
 * See the LICENSE file for details.
 */

#include "aerell/compiler/token/token.h"
#include <aerell/support/ostream.h>

namespace aerell
{

std::string_view Token::getText() const { return {this->source->getContentData() + this->offset, this->size}; }

void print(const Token::Vec& vec)
{
    const char* path = nullptr;
    size_t maxWidth = 0;

    for(const auto& token : vec)
    {
        const char* t = to_string(token.type);
        size_t width = std::strlen(t);
        maxWidth = std::max(maxWidth, width);
    }

    for(const auto& token : vec)
    {
        auto sourcePath = token.source->getPath().c_str();
        if(path != sourcePath)
        {
            path = sourcePath;
            outs() << '\n' << path << ":\n";
        }

        const char* t = to_string(token.type);
        size_t width = std::strlen(t);

        if(width < maxWidth)
        {
            outs() << std::string(maxWidth - width, ' ');
        }

        outs() << t << " " << token.getText() << '\n';
    }
}

void print(const Token::Vecs& vecs)
{
    for(const auto& vec : vecs) print(vec);
}

} // namespace aerell