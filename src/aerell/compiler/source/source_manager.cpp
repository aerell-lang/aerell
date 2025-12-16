#include "aerell/compiler/source/source_manager.h"

namespace Aerell
{

bool SourceManager::import(const char* path)
{
    if(this->contain(path)) return true;
    Source source{path};
    if(!source.exist()) return false;
    if(!source.read()) return false;
    this->sources.push_back(std::move(source));
    return true;
}

bool SourceManager::contain(const char* path)
{
    for(const auto& source : this->sources)
        if(source.getPath() == path) return true;
    return false;
}

Source* SourceManager::getLastSource() { return &this->sources.back(); }

bool SourceManager::hasSource() { return !this->sources.empty(); }

} // namespace Aerell