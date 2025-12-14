#include <fstream>
#include <iostream>

#include "llvm/Support/raw_ostream.h"

#include <aerell/compiler/lexer/lexer.h>
#include <aerell/compiler/parser/parser.h>
#include "aerell/compiler/ir/ir.h"
#include "aerell/compiler/codegen/codegen.h"
#include "aerell/linker/linker.h"

int main(int argc, char* argv[])
{
    if(argc == 1) return 1;

    // Read file
    std::ifstream file(argv[1], std::ios::binary);
    if(!file.is_open())
    {
        llvm::errs() << "Failed to open file '" << argv[1] << "'.\n";
        return 1;
    }

    std::string content;

    {
        std::vector<unsigned char> fileBytes((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());

        file.close();

        content = {fileBytes.begin(), fileBytes.end()};
    }

    // Lexer
    auto tokens = aerell::Lexer::gen(content);
    // aerell::print(tokens);

    // Parser
    auto asts = aerell::Parser::gen(tokens);
    // aerell::print(asts);

    // IR Gen
    auto module = aerell::IR::gen(asts);
    aerell::IR::optimize(module);

    // aerell::print(module);

    if(!aerell::IR::verify(module, &llvm::outs())) return 1;

    // Code Gen
    aerell::CodeGen::initialize();

    const char* objFileName = "a.o";
    aerell::CodeGen::object(module, objFileName);

    // Linker
    aerell::Linker::initialize();
    aerell::Linker::linking(objFileName);

    return 0;
}