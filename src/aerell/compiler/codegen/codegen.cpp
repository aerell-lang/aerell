#include "llvm/IR/LegacyPassManager.h"
#include "llvm/MC/TargetRegistry.h"
#include "llvm/Target/TargetMachine.h"
#include "llvm/Target/TargetOptions.h"
#include "llvm/TargetParser/Host.h"
#include <llvm/Support/TargetSelect.h>

#include "aerell/compiler/codegen/codegen.h"

namespace aerell
{

void CodeGen::initialize()
{
    llvm::InitializeNativeTarget();
    llvm::InitializeNativeTargetAsmParser();
    llvm::InitializeNativeTargetAsmPrinter();
    llvm::InitializeNativeTargetDisassembler();
}

bool CodeGen::object(const std::unique_ptr<llvm::Module>& module, const char* filename)
{
    auto targetTriple = llvm::Triple(llvm::sys::getDefaultTargetTriple());
    module->setTargetTriple(targetTriple);

    std::string error;
    auto target = llvm::TargetRegistry::lookupTarget(targetTriple, error);
    if(target == nullptr)
    {
        llvm::errs() << error << "\n";
        return false;
    }

    llvm::TargetOptions opt;
    auto RM = std::optional<llvm::Reloc::Model>();
    llvm::TargetMachine* targetMachine = target->createTargetMachine(
        targetTriple, "generic", "", opt, RM, std::nullopt, llvm::CodeGenOptLevel::Aggressive);

    std::error_code EC;
    llvm::raw_fd_ostream dest(filename, EC, llvm::sys::fs::OpenFlags());

    if(EC)
    {
        llvm::errs() << "Could not open file: " << EC.message() << "\n";
        return false;
    }

    llvm::legacy::PassManager pass;
    auto fileType = llvm::CodeGenFileType::ObjectFile; // bisa juga asm: CGFT_AssemblyFile
    if(targetMachine->addPassesToEmitFile(pass, dest, nullptr, fileType))
    {
        llvm::errs() << "TargetMachine can't emit a file of this type\n";
        return false;
    }

    pass.run(*module);
    dest.flush();

    return true;
}

} // namespace aerell