#include "aerell/compiler/symbol/symbol_func.h"
#include <aerell/compiler/symbol/symbol_type.h>

namespace Aerell
{

SymbolFunc::SymbolFunc(bool pub) : Symbol(SymbolType::FUNC), pub(pub) {}

SymbolFunc::~SymbolFunc() {}

bool SymbolFunc::getPub() const { return this->pub; }

void SymbolFunc::setVrdic(bool value) { this->vrdic = value; }

bool SymbolFunc::getVrdic() const { return this->vrdic; }

void SymbolFunc::setRet(Type type) { this->ret = type; }

Type SymbolFunc::getRet() const { return this->ret; }

void SymbolFunc::setParams(std::vector<Type>&& types) { this->params = std::move(types); }

const std::vector<Type>& SymbolFunc::getParams() const { return this->params; }

void SymbolFunc::setScope(SymbolTable* scope) { this->scope = scope; }

} // namespace Aerell