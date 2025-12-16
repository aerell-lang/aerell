#include <llvm/Support/raw_ostream.h>

#include "aerell/compiler/token/token.h"

namespace Aerell
{

void print(const Token& token)
{
    llvm::outs() << "[" << to_string(token.type) << "] "
                 << std::string_view{token.source->getContent().data() + token.offset, token.size} << "\n";
}

void print(const std::vector<Token>& tokens)
{
    for(const auto& token : tokens) print(token);
}

} // namespace Aerell