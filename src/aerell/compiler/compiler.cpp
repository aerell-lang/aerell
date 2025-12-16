#include <fstream>
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

std::optional<std::string> Compiler::read(const char* filePath)
{
    // Read file
    std::ifstream file(filePath, std::ios::binary);
    if(!file.is_open())
    {
        llvm::errs() << "Failed to open file.\n";
        return std::nullopt;
    }

    std::vector<unsigned char> fileBytes((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());

    file.close();

    return {{fileBytes.begin(), fileBytes.end()}};
}

std::unique_ptr<llvm::Module> Compiler::genIR(const std::string& fileContent)
{
    // Lexer
    auto tokens = Aerell::Lexer::gen(fileContent);

    // Parser
    auto asts = Aerell::Parser::gen(tokens);

    // IR Gen
    auto module = Aerell::IR::gen(asts);
    Aerell::IR::optimize(module);

    if(!Aerell::IR::verify(module, &llvm::outs())) return nullptr;

    return module;
}

std::unique_ptr<llvm::Module> Compiler::genIR(const char* filePath)
{
    auto fileContent = read(filePath);
    if(!fileContent.has_value()) return nullptr;
    return genIR(fileContent.value());
}

bool Compiler::compile(const char* filePath)
{
    // IR Gen
    auto module = genIR(filePath);

    // Code Gen
    return Aerell::CodeGen::obj(filePath, module);
}

} // namespace Aerell