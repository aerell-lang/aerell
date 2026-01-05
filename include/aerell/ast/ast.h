/*
 * This file is part of The Aerell Programming Language
 *
 * Copyright 2025 The Aerell Programming Language Authors
 * Licensed under the Apache License, Version 2.0.
 * See the LICENSE file for details.
 */

#ifndef AERELL_AST_AST_H
#define AERELL_AST_AST_H

#include <memory>
#include <vector>

#include "aerell/support/ostream.h"

namespace aerell
{

class AST
{
  public:
    using Ptr = std::unique_ptr<AST>;
    using List = std::vector<Ptr>;

    virtual ~AST() {};

    virtual void print(OStream& os, size_t indent = 0) const = 0;
};

inline OStream& operator<<(OStream& os, const AST& obj)
{
    obj.print(os);
    return os;
}

inline OStream& operator<<(OStream& os, const AST::Ptr& ast)
{
    os << (*ast);
    return os;
}

inline OStream& operator<<(OStream& os, const AST::List& list)
{
    for(const auto& ast : list) os << ast;
    return os;
}

} // namespace aerell

#endif