#include "aerell/compiler/source/source_manager.h"
#include <memory>

namespace Aerell
{

bool SourceManager::import(const char* path, llvm::raw_ostream& errs)
{
    Source source{path};
    if(!source.exist(errs)) return false;
    if(!source.read(errs)) return false;
    this->sources.push_back(std::make_unique<Source>(std::move(source)));
    return true;
}

bool SourceManager::contain(const char* path)
{
    for(const auto& source : this->sources)
        if(source->getPath() == path) return true;
    return false;
}

Source* SourceManager::getFirstSource() { return this->sources.front().get(); }
Source* SourceManager::getLastSource() { return this->sources.back().get(); }

bool SourceManager::hasSource() { return !this->sources.empty(); }

} // namespace Aerell