#include <memory>

#include "aerell/compiler/compiler.h"
#include <aerell/compiler/semantic/semantic.h>
#include <aerell/compiler/ir/ir.h>
#include <aerell/compiler/codegen/codegen.h>

#include <llvm/Support/raw_ostream.h>

namespace Aerell
{

std::vector<std::vector<Token>> Compiler::genTokenss(Source* source)
{
    if(mainSource == nullptr) mainSource = source;
    std::vector<std::vector<Token>> tokenss;

    size_t x = 0;

    auto tokens = this->lexer.gen(source);
    for(auto token : tokens)
    {
        if(token.type != TokenType::STRL) break;
        x++;

        auto filePath = std::filesystem::path(mainSource->getPath())
                            .replace_filename(
                                std::string_view{token.source->getContent().data() + token.offset + 1, token.size - 2})
                            .replace_extension("arl")
                            .generic_string();

        if(sourceManager.contain(filePath.c_str())) continue;

        std::string errorMessage;
        llvm::raw_string_ostream os(errorMessage);
        if(!sourceManager.import(filePath.c_str(), os))
        {
            token.source->printErrorMessage(token.offset, token.size, errorMessage.c_str());
            continue;
        }

        for(auto& token : genTokenss(sourceManager.getLastSource())) tokenss.push_back(std::move(token));
    }

    tokenss.emplace_back(std::make_move_iterator(tokens.begin() + x), std::make_move_iterator(tokens.end()));
    return tokenss;
}

std::vector<std::unique_ptr<llvm::Module>> Compiler::genIR(Source* source)
{
    // Lexer
    auto tokenss = genTokenss(source);

    std::vector<std::unique_ptr<llvm::Module>> modules;
    for(const auto& tokens : tokenss)
    {
        if(tokens.empty()) continue;

        // Parser
        std::vector<std::unique_ptr<AST>> asts;
        if(!this->parser.gen(tokens, asts)) continue;

        // Semantic
        if(!this->semantic.analysis(asts)) continue;

        // IR Gen
        std::unique_ptr<llvm::Module> module = nullptr;
        if(!this->ir.gen(tokens.front().source->getPath().c_str(), asts, module)) continue;

        modules.push_back(std::move(module));
    }

    return modules;
}

std::vector<std::unique_ptr<llvm::Module>> Compiler::genIR(const char* filePath)
{
    if(sourceManager.contain(filePath) || !sourceManager.import(filePath, llvm::errs()) || !sourceManager.hasSource())
        return {};
    return genIR(sourceManager.getLastSource());
}

bool Compiler::compile(const char* filePath, std::vector<std::string>& outputs)
{
    // IR Gen
    auto modules = genIR(filePath);
    if(modules.empty()) return false;

    // Code Gen
    std::vector<bool> success;
    for(const auto& module : modules)
    {
        const auto& filePath = module->getSourceFileName();
        bool status = Aerell::CodeGen::obj(filePath.c_str(), module);
        if(status) outputs.push_back(filePath);
        success.push_back(status);
    }

    return std::all_of(success.begin(), success.end(), [](const auto& status) { return status == true; });
}

} // namespace Aerell