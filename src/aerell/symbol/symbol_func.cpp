/*
 * This file is part of The Aerell Programming Language
 *
 * Copyright 2025 The Aerell Programming Language Authors
 * Licensed under the Apache License, Version 2.0.
 * See the LICENSE file for details.
 */

#include "aerell/symbol/symbol_func.h"
#include "aerell/symbol/symbol_type.h"

namespace aerell
{

SymbolFunc::SymbolFunc(SymbolTable* scope, bool pub) : Symbol(scope, SymbolType::FUNC), pub(pub) {}

SymbolFunc::~SymbolFunc() {}

const bool& SymbolFunc::getPub() const { return this->pub; }

void SymbolFunc::setVrdic(bool value) { this->vrdic = value; }

const bool& SymbolFunc::getVrdic() const { return this->vrdic; }

void SymbolFunc::setRet(IRType type) { this->ret = type; }

const std::optional<IRType>& SymbolFunc::getRet() const { return this->ret; }

void SymbolFunc::setParams(std::vector<IRType>&& types) { this->params = std::move(types); }

const std::vector<IRType>& SymbolFunc::getParams() const { return this->params; }

void SymbolFunc::setBlockScope(SymbolTable* blockScope) { this->blockScope = blockScope; }

const SymbolTable* SymbolFunc::getBlockScope() const { return this->blockScope; }

} // namespace aerell