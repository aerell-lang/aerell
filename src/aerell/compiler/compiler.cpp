#include <aerell/compiler/compiler.h>
#include <memory>

#include <aerell/support/utils.h>
#include "aerell/compiler/compiler.h"
#include <aerell/compiler/semantic/semantic.h>
#include <aerell/compiler/ir/ir.h>
#include <aerell/compiler/codegen/codegen.h>

#include <llvm/Support/raw_ostream.h>

namespace Aerell
{

std::optional<std::string> Compiler::findFilePathFromName(std::string_view mainSourcePath, std::string_view fileName)
{
    static std::string stdSourcePath;
    if(stdSourcePath.empty()) stdSourcePath = getExeDir().append("../src/std.arl").generic_string();

    std::vector<std::string_view> paths{mainSourcePath, stdSourcePath};

    for(const auto& path : paths)
    {
        auto filePath = std::filesystem::path(path).replace_filename(fileName).replace_extension("arl");
        if(std::filesystem::exists(filePath)) return filePath.generic_string();
    }

    return std::nullopt;
}

Compiler::Tokens Compiler::lexing(const char* mainSourcePath, Source* source)
{
    if(mainSourcePath == nullptr) mainSourcePath = source->getPath().c_str();

    // Tokenize
    Tokens cTokens;
    size_t lastImportIndex = 0;

    auto mainTokens = this->lexer.lexing(source);
    for(auto token : mainTokens)
    {
        if(token.type != TokenType::STRL) break;
        lastImportIndex++;

        auto filePath = this->findFilePathFromName(mainSourcePath, token.getText().substr(1, token.size - 2));
        if(!filePath.has_value())
        {
            token.source->printErrorMessage(token.offset, token.size, "File does not exist");
            continue;
        }

        std::string errorMessage;
        llvm::raw_string_ostream os(errorMessage);
        if(!sourceManager.import(filePath.value().c_str(), os) && !errorMessage.empty())
        {
            token.source->printErrorMessage(token.offset, token.size, errorMessage.c_str());
            continue;
        }

        for(auto& token : lexing(mainSourcePath, sourceManager.getLastSource())) cTokens.push_back(std::move(token));
    }

    cTokens.emplace_back(
        std::make_move_iterator(mainTokens.begin() + lastImportIndex), std::make_move_iterator(mainTokens.end()));

    return cTokens;
}

Compiler::Tokens Compiler::lexing(Source* source)
{
    // Tokenize
    return this->lexing(nullptr, source);
}

Compiler::Tokens Compiler::lexing(const char* filePath)
{
    // Import file
    if(!this->sourceManager.import(filePath, llvm::outs())) return {};

    // Tokenize
    return this->lexing(this->sourceManager.getLastSource());
}

bool Compiler::parsing(const Tokens& cTokens, Asts& cAsts)
{
    bool hasError = false;

    for(const Lexer::Tokens& tokens : cTokens)
    {
        if(tokens.empty()) continue;

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

bool Compiler::generating(const Tokens& tokens, const Asts& cAsts, Modules& modules)
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

    return !hasError;
}

bool Compiler::compile(const Modules& modules, std::vector<std::string>& outputs)
{
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

void print(const Compiler::Tokens& cTokens)
{
    for(const Lexer::Tokens& tokens : cTokens) print(tokens);
}

void print(const Compiler::Asts& cAsts)
{
    for(const AST::Asts& asts : cAsts) print(asts);
}

void print(const Compiler::Modules& modules)
{
    for(const IR::Module& module : modules) print(module);
}

} // namespace Aerell