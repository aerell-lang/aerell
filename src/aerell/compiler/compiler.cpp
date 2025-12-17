#include <aerell/compiler/semantic/semantic.h>
#include <memory>

#include "aerell/compiler/compiler.h"
#include <aerell/compiler/lexer/lexer.h>
#include <aerell/compiler/parser/parser.h>
#include <aerell/compiler/ir/ir.h>
#include <aerell/compiler/codegen/codegen.h>

#include <llvm/Support/raw_ostream.h>

#if defined(_WIN32)
#include <windows.h>
#endif

namespace Aerell
{

SourceManager Compiler::sourceManager;

std::unique_ptr<llvm::Module> Compiler::genIR(Source* source)
{
    // Lexer
    auto tokens = Aerell::Lexer::gen(source);

    // Parser
    std::vector<std::unique_ptr<AST>> asts;
    if(!Aerell::Parser::gen(tokens, asts)) return nullptr;

    // Semantic
    if(!Aerell::Semantic::analysis(asts)) return nullptr;

    // IR Gen
    std::unique_ptr<llvm::Module> module = nullptr;
    if(!Aerell::IR::gen(asts, module)) return nullptr;

    return module;
}

std::unique_ptr<llvm::Module> Compiler::genIR(const char* filePath)
{
    if(!sourceManager.import(filePath) || !sourceManager.hasSource()) return nullptr;
    return genIR(sourceManager.getLastSource());
}

bool Compiler::compile(const char* filePath)
{
    // IR Gen
    auto module = genIR(filePath);
    if(module == nullptr) return false;

    // Code Gen
    return Aerell::CodeGen::obj(filePath, module);
}

} // namespace Aerell