#pragma once

#include <memory>

#include <aerell/compiler/source/source.h>

#include <llvm/Support/raw_ostream.h>

namespace Aerell
{

class SourceManager
{
  public:
    bool import(const char* sourcePath, llvm::raw_ostream& errs);
    Source* getLastSource();
    bool hasSource();

  private:
    std::vector<std::unique_ptr<Source>> sources;
    bool contain(const char* sourcePath);
};

} // namespace Aerell