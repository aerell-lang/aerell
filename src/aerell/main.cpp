#include <fstream>

#include <aerell/compiler/lexer/lexer.h>
#include <aerell/compiler/parser/parser.h>
#include "aerell/compiler/ir/ir.h"
#include "aerell/compiler/codegen/codegen.h"
#include "aerell/linker/linker.h"
#include "aerell/support/argparse.h"

int main(int argc, char* argv[])
{
    argparse::ArgumentParser program("aerell", "0.0.0");
    program.add_description("A command-line utility for Aerell development.");

    argparse::ArgumentParser compileCommand("compile", "", argparse::default_arguments::help);
    compileCommand.add_description("Compile the Aerell source file.");
    auto& compileCommandType = compileCommand.add_mutually_exclusive_group();
    compileCommandType.add_argument("-S").flag().help("Generate assembly code (e.g., .s file)");
    compileCommandType.add_argument("-c").flag().help("Compile or assemble only, but do not link (e.g., .o file)");
    compileCommand.add_argument<std::string>("file").help("Aerell source file to compile.").required();
    compileCommand.add_argument<std::string>("-o").help("Specify the required output file name.").required();

    program.add_subparser(compileCommand);

    if(argc <= 1)
    {
        llvm::outs() << program.help().str() << "\n";
        return 0;
    }

    try
    {
        program.parse_args(argc, argv);
    }
    catch(const std::exception& err)
    {
        if(argc > 1 && std::string(argv[1]).compare("compile") == 0)
            llvm::errs() << err.what() << "\n" << compileCommand.help().str() << "\n";
        else
            llvm::errs() << err.what() << "\n" << program.help().str() << "\n";
        return 1;
    }

    if(program.is_subcommand_used("compile"))
    {
        auto input = compileCommand.get<std::string>("file");
        auto output = compileCommand.get<std::string>("-o");
        bool generateAssembly = compileCommand.get<bool>("-S");
        bool generateObject = compileCommand.get<bool>("-c");

        // Read file
        std::ifstream file(input, std::ios::binary);
        if(!file.is_open())
        {
            llvm::errs() << "Failed to open file '" << input << "'.\n";
            return 1;
        }

        std::string content;

        {
            std::vector<unsigned char> fileBytes(
                (std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());

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
        aerell::CodeGen::gen(
            generateAssembly ? aerell::CodeGen::Type::ASM : aerell::CodeGen::Type::OBJ, output.c_str(), module);

        if(generateAssembly || generateObject) return 0;

        // Linker
        aerell::Linker::initialize();
        aerell::Linker::linking(output.c_str());
    }

    return 0;
}