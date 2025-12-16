#include <filesystem>

#include "llvm/IR/LegacyPassManager.h"
#include "llvm/MC/TargetRegistry.h"
#include "llvm/Target/TargetMachine.h"
#include "llvm/Target/TargetOptions.h"
#include "llvm/TargetParser/Host.h"

#include "aerell/compiler/codegen/codegen.h"

namespace Aerell
{

bool CodeGen::obj(const char* name, const std::unique_ptr<llvm::Module>& module)
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
    llvm::raw_fd_ostream dest(
        std::filesystem::path(name).filename().replace_extension("o").string(), EC, llvm::sys::fs::OpenFlags());

    if(EC)
    {
        llvm::errs() << "Could not open file: " << EC.message() << "\n";
        return false;
    }

    llvm::legacy::PassManager pass;
    if(targetMachine->addPassesToEmitFile(pass, dest, nullptr, llvm::CodeGenFileType::ObjectFile))
    {
        llvm::errs() << "TargetMachine can't emit a file of this type\n";
        return false;
    }

    pass.run(*module);
    dest.flush();

    return true;
}

} // namespace Aerell