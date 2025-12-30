/*
 * This file is part of The Aerell Programming Language
 *
 * Copyright 2025 The Aerell Programming Language Authors
 * Licensed under the Apache License, Version 2.0.
 * See the LICENSE file for details.
 */

#ifndef AERELL_COMPILER_AST_AST_H
#define AERELL_COMPILER_AST_AST_H

#include <memory>
#include <vector>

#include <llvm/Support/raw_ostream.h>

namespace aerell
{

class AST
{
  public:
    using Ptr = std::unique_ptr<AST>;
    using Children = std::vector<Ptr>;

    struct ChildrenWithSource
    {
        const char* source = nullptr;
        Children children;
    };

    using Groups = std::vector<ChildrenWithSource>;

    virtual ~AST() {};

    virtual void print(llvm::raw_ostream& os, size_t indent = 0) const = 0;
};

inline llvm::raw_ostream& operator<<(llvm::raw_ostream& os, const AST& obj)
{
    obj.print(os);
    return os;
}

inline llvm::raw_ostream& operator<<(llvm::raw_ostream& os, const AST::Ptr& obj)
{
    os << (*obj);
    return os;
}

inline llvm::raw_ostream& operator<<(llvm::raw_ostream& os, const AST::Children& children)
{
    for(const auto& child : children) os << child;
    return os;
}

inline llvm::raw_ostream& operator<<(llvm::raw_ostream& os, const AST::ChildrenWithSource& childrenWithSource)
{
    os << '\n' << childrenWithSource.source << ":\n" << childrenWithSource.children;
    return os;
}

inline llvm::raw_ostream& operator<<(llvm::raw_ostream& os, const AST::Groups& groups)
{
    for(const auto& group : groups) os << group;
    return os;
}

} // namespace aerell

#endif