#include "aerell/compiler/symbol/symbol_var.h"
#include <aerell/compiler/symbol/symbol_type.h>

namespace Aerell
{

SymbolVar::SymbolVar() : Symbol(SymbolType::VAR) {}

SymbolVar::~SymbolVar() {}

void SymbolVar::setType(Type type) { this->type = type; }

} // namespace Aerell