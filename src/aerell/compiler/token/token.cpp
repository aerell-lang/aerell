#include <llvm/Support/raw_ostream.h>

#include "aerell/compiler/token/token.h"

namespace aerell
{

void print(const Token& token) { llvm::outs() << "[" << to_string(token.type) << "] " << token.content << "\n"; }

void print(const std::vector<Token>& tokens)
{
    for(const auto& token : tokens) print(token);
}

} // namespace aerell