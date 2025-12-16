#pragma once

#include <aerell/compiler/source/source.h>

#include <llvm/Support/raw_ostream.h>

namespace Aerell
{

class SourceManager
{
  public:
    bool import(const char* path);
    Source* getLastSource();
    bool hasSource();

  private:
    std::vector<Source> sources;

    bool contain(const char* path);
};

} // namespace Aerell