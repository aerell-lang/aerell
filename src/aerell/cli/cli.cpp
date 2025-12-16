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
    llvm::outs() << " run <file>        Run a Aerell program.\n";
    llvm::outs() << " compile <file>    Compile Aerell to executable.\n";
}

} // namespace Aerell

int main(int argc, char* argv[])
{
    llvm::InitializeNativeTarget();
    llvm::InitializeNativeTargetAsmParser();
    llvm::InitializeNativeTargetAsmPrinter();
    llvm::InitializeNativeTargetDisassembler();

    if(argc == 1)
    {
        Aerell::CLI::printHelp();
        return EXIT_SUCCESS;
    }

    if(argc == 2 && std::strcmp(argv[1], "help") == 0)
    {
        Aerell::CLI::printHelp();
        return EXIT_SUCCESS;
    }

    if(argc == 2 && std::strcmp(argv[1], "version") == 0)
    {
        Aerell::CLI::printVersion();
        return EXIT_SUCCESS;
    }

    if(argc == 3 && std::strcmp(argv[1], "run") == 0)
    {
        const auto& file = argv[2];

        // JIT
        if(!Aerell::Compiler::jit(file)) return EXIT_FAILURE;

        return EXIT_SUCCESS;
    }

    if(argc == 3 && std::strcmp(argv[1], "compile") == 0)
    {
        const auto& file = argv[2];

        // Compile
        if(!Aerell::Compiler::compile(file)) return EXIT_FAILURE;

        // Linker
        Aerell::Linker::initialize();
        if(!Aerell::Linker::linking(file)) return EXIT_FAILURE;

        llvm::outs() << "Successfully compiled the file.";
        return EXIT_SUCCESS;
    }

    llvm::outs() << "Invalid arguments or too few or too many.\n";
    Aerell::CLI::printHelp();

    return EXIT_FAILURE;
}