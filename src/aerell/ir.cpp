// Copyright 2026 Fern Aerell.
// SPDX-License-Identifier: Apache-2.0

#include "aerell/ir.hpp"

namespace aerell
{

IR::IR(AST& ast) : ast{ast} {}

Module IR::gen()
{
    this->module.setName(this->ast.getFile().getPath().generic_string());

    auto& initLabel = this->module.addLabel("init", true);

    std::vector<std::uint32_t> indexs{1};
    while(!indexs.empty())
    {
        auto index = indexs.back();
        indexs.pop_back();

        auto kind = this->ast.getKind(index);
        switch(kind)
        {
        case ASTKind::NONE: break;
        case ASTKind::ROOT:
        case ASTKind::STMT:
        case ASTKind::EXPR:
            indexs.push_back(this->ast.getData2(index));
            indexs.push_back(this->ast.getData1(index));
            break;
        case ASTKind::INTL:
            initLabel.getInst().addInst(
                InstKind::PRINT,
                this->ast.getFile().getLexemeText({this->ast.getData1(index), this->ast.getData2(index)}));
            break;
        }
    }

    return std::move(this->module);
}

} // namespace aerell