/*
 * This file is part of The Aerell Programming Language
 *
 * Copyright 2025 The Aerell Programming Language Authors
 * Licensed under the Apache License, Version 2.0.
 * See the LICENSE file for details.
 */

#include "aerell/compiler/symbol/symbol_func.h"
#include <aerell/compiler/symbol/symbol_type.h>

namespace aerell
{

SymbolFunc::SymbolFunc(SymbolTable* scope, bool pub) : Symbol(scope, SymbolType::FUNC), pub(pub) {}

SymbolFunc::~SymbolFunc() {}

bool SymbolFunc::getPub() const { return this->pub; }

void SymbolFunc::setVrdic(bool value) { this->vrdic = value; }

bool SymbolFunc::getVrdic() const { return this->vrdic; }

void SymbolFunc::setRet(DataType dataType) { this->ret = dataType; }

const std::optional<DataType>& SymbolFunc::getRet() const { return this->ret; }

void SymbolFunc::setParams(std::vector<DataType>&& dataTypes) { this->params = std::move(dataTypes); }

const std::vector<DataType>& SymbolFunc::getParams() const { return this->params; }

void SymbolFunc::setBlockScope(SymbolTable* blockScope) { this->blockScope = blockScope; }

const SymbolTable* SymbolFunc::getBlockScope() const { return this->blockScope; }

} // namespace aerell