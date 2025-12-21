#pragma once

#include <memory>

#include <aerell/compiler/source/source.h>

#include <llvm/Support/raw_ostream.h>

namespace Aerell
{

class SourceManager
{
  public:
    bool import(const char* path, llvm::raw_ostream& errs);
    bool contain(const char* path);
    Source* getFirstSource();
    Source* getLastSource();
    bool hasSource();

  private:
    std::vector<std::unique_ptr<Source>> sources;
};

} // namespace Aerell