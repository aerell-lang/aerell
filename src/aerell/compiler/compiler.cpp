#include <memory>

#include <aerell/support/utils.h>
#include <aerell/compiler/compiler.h>
#include <aerell/compiler/semantic/semantic.h>
#include <aerell/compiler/ir/ir.h>
#include <aerell/compiler/codegen/codegen.h>

#include <llvm/Support/raw_ostream.h>

namespace Aerell
{

Compiler::Tokens Compiler::lexing(Source* source)
{
    // Tokenize
    Tokens cTokens;
    size_t lastImportIndex = 0;

    auto mainTokens = this->lexer.lexing(source);

    for(auto token : mainTokens)
    {
        if(token.type != TokenType::STRL) break;
        lastImportIndex++;

        std::string errorMessage;
        llvm::raw_string_ostream os(errorMessage);
        int status = sourceManager.import(std::string(token.getText().substr(1, token.size - 2)).c_str(), os);
        if(status == 0)
        {
            errorMessage = {"[I] " + errorMessage};
            token.source->printErrorMessage(token.offset, token.size, errorMessage.c_str());
            continue;
        }
        else if(status == 2)
            continue;

        for(auto& token : lexing(sourceManager.getLastSource())) cTokens.push_back(std::move(token));
    }

    cTokens.emplace_back(
        std::make_move_iterator(mainTokens.begin() + lastImportIndex), std::make_move_iterator(mainTokens.end()));

    return cTokens;
}

Compiler::Tokens Compiler::lexing(const char* filePath)
{
    Compiler::Tokens mainTokens;

    std::vector<std::pair<const char*, bool>> autoImportSources{
        {"std/std", false},
        {filePath, true},
    };

    for(const auto& autoImportSource : autoImportSources)
    {
        // Import file
        int status = this->sourceManager.import(autoImportSource.first, llvm::errs(), autoImportSource.second);
        if(status == 0)
        {
            llvm::errs() << '\n';
            continue;
        }
        else if(status == 2)
            continue;

        // Lexing
        for(auto& tokens : this->lexing(this->sourceManager.getLastSource())) mainTokens.push_back(std::move(tokens));
    }

    return mainTokens;
}

bool Compiler::parsing(const Tokens& cTokens, Asts& cAsts)
{
    bool hasError = false;

    for(const Lexer::Tokens& tokens : cTokens)
    {
        AST::Asts asts;
        if(!this->parser.parsing(tokens, asts))
        {
            if(!hasError) hasError = true;
            continue;
        }

        cAsts.push_back(std::move(asts));
    }

    return !hasError;
}

bool Compiler::analysis(const Asts& cAsts)
{
    bool hasError = false;

    for(const AST::Asts& asts : cAsts)
    {
        if(!this->semantic.analysis(asts))
            if(!hasError) hasError = true;
    }

    return !hasError;
}

bool Compiler::generating(const Tokens& tokens, const Asts& cAsts, IR::Modules& modules)
{
    bool hasError = false;

    size_t index = 0;
    for(const AST::Asts& asts : cAsts)
    {
        auto sourceFileName = tokens[index].front().source->getPath().c_str();
        index++;
        IR::Module module = nullptr;
        if(!this->ir.generating(sourceFileName, asts, module))
        {
            if(!hasError) hasError = true;
            continue;
        }

        modules.push_back(std::move(module));
    }

    auto startModule = this->ir.getStartModule();
    if(startModule == nullptr && !hasError) hasError = true;
    if(!this->ir.linking(startModule, modules[std::max(0, ((int)modules.size()) - 1)]) && !hasError) hasError = true;

    return !hasError;
}

IR::Module Compiler::linking(IR::Modules& modules)
{
    auto module = this->ir.linking(modules);
    if(module == nullptr) return nullptr;
    this->ir.optimize(module);
    return module;
}

bool Compiler::compile(IR::Modules& modules, std::vector<std::string>& outputs)
{
    if(modules.empty()) return false;

    // Code Gen
    std::vector<bool> success;
    for(auto& module : modules)
    {
        this->ir.optimize(module);
        const auto& filePath = module->getSourceFileName();
        bool status = Aerell::CodeGen::obj(filePath.c_str(), module);
        if(status) outputs.push_back(filePath);
        success.push_back(status);
    }

    return std::all_of(success.begin(), success.end(), [](const auto& status) { return status == true; });
}

std::optional<std::string> Compiler::compile(IR::Modules& modules)
{
    auto module = this->linking(modules);
    if(module == nullptr) return std::nullopt;
    const auto& filePath = module->getSourceFileName();
    if(!Aerell::CodeGen::obj(filePath.c_str(), module)) return std::nullopt;
    return filePath;
}

void print(const Compiler::Tokens& cTokens)
{
    for(const Lexer::Tokens& tokens : cTokens) print(tokens);
}

void print(const Compiler::Asts& cAsts)
{
    for(const AST::Asts& asts : cAsts) print(asts);
}

void print(const IR::Modules& modules)
{
    for(const IR::Module& module : modules) print(module);
}

} // namespace Aerell