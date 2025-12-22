#pragma once

#include <memory>

#include <aerell/support/utils.h>
#include <aerell/compiler/source/source.h>

#include <llvm/Support/raw_ostream.h>

namespace Aerell
{

class SourceManager
{
  public:
    int import(const char* value, llvm::raw_ostream& errs, bool makeAsPrefix = false);
    Source* getLastSource();
    bool hasSource();

  private:
    std::vector<std::string> importSourcePrefixs{getExeDir().append("../src").generic_string()};
    std::vector<std::unique_ptr<Source>> sources;
    bool contain(const std::filesystem::path& path);
    std::optional<std::filesystem::path> find(const char* value);
};

} // namespace Aerell