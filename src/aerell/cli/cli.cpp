#include <cstdlib>
#include <cstring>

#include "aerell/cli/cli.h"
#include <aerell/linker/linker.h>
#include <aerell/compiler/compiler.h>

#include <llvm/Support/raw_ostream.h>
#include <llvm/Support/TargetSelect.h>

namespace Aerell
{

void CLI::printVersion() { llvm::outs() << "Aerell version: 0.0.0"; }

void CLI::printHelp()
{
    llvm::outs() << "A command-line utility for Aerell development.\n";
    llvm::outs() << "\n";
    llvm::outs() << "Usage: aerell <command>\n";
    llvm::outs() << "\n";
    llvm::outs() << "Available commands:\n";
    llvm::outs() << " help              Print this usage information.\n";
    llvm::outs() << " version           Print the Aerell version.\n";
    llvm::outs() << " run <file>        Run an Aerell file.\n";
    llvm::outs() << " compile <file>    Compile an Aerell file to executable.\n";
}

} // namespace Aerell

int main(int argc, char* argv[])
{
    llvm::InitializeNativeTarget();
    llvm::InitializeNativeTargetAsmParser();
    llvm::InitializeNativeTargetAsmPrinter();
    llvm::InitializeNativeTargetDisassembler();

    Aerell::CLI cli;
    Aerell::Compiler compiler;
    Aerell::Linker linker;

    if(argc == 1)
    {
        cli.printHelp();
        return EXIT_SUCCESS;
    }

    if(argc == 2 && std::strcmp(argv[1], "help") == 0)
    {
        cli.printHelp();
        return EXIT_SUCCESS;
    }

    if(argc == 2 && std::strcmp(argv[1], "version") == 0)
    {
        cli.printVersion();
        return EXIT_SUCCESS;
    }

    if(argc == 3 && std::strcmp(argv[1], "run") == 0)
    {
        const auto& file = argv[2];

        // JIT
        if(!compiler.jit(file)) return EXIT_FAILURE;

        return EXIT_SUCCESS;
    }

    if(argc == 3 && std::strcmp(argv[1], "compile") == 0)
    {
        const auto& file = argv[2];

        // Compile
        std::vector<std::string> files;
        if(!compiler.compile(file, files)) return EXIT_FAILURE;

        // Linker
        if(!linker.linking(files)) return EXIT_FAILURE;

        llvm::outs() << "Successfully compiled the file.";
        return EXIT_SUCCESS;
    }

    llvm::outs() << "Invalid arguments or too few or too many.\n";
    cli.printHelp();

    return EXIT_FAILURE;
}