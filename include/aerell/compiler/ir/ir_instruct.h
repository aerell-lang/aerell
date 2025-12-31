/*
 * This file is part of The Aerell Programming Language
 *
 * Copyright 2025 The Aerell Programming Language Authors
 * Licensed under the Apache License, Version 2.0.
 * See the LICENSE file for details.
 */

#ifndef AERELL_COMPILER_IR_IR_INSTRUCT_H
#define AERELL_COMPILER_IR_IR_INSTRUCT_H

#include <memory>
#include <vector>

#include <aerell/support/ostream.h>
#include <aerell/compiler/ir/ir_val.h>

namespace aerell
{

class IRInstruct : public IRVal
{
  public:
    using Ptr = std::unique_ptr<IRInstruct>;
    using Vec = std::vector<Ptr>;

    virtual ~IRInstruct() {}

    virtual void print(OStream& os) const = 0;
};

inline OStream& operator<<(OStream& os, const IRInstruct& obj)
{
    obj.print(os);
    return os;
}

inline OStream& operator<<(OStream& os, const IRInstruct::Ptr& obj)
{
    os << (*obj);
    return os;
}

inline OStream& operator<<(OStream& os, const IRInstruct::Vec& obj)
{
    for(const auto& ptr : obj) os << ptr;
    return os;
}

} // namespace aerell

#endif