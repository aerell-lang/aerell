#include <algorithm>
#include <memory>

#include "aerell/compiler/source/source_manager.h"

namespace Aerell
{

bool SourceManager::import(const char* sourcePath, llvm::raw_ostream& errs)
{
    if(this->contain(sourcePath)) return false;

    Source source{sourcePath};
    if(!source.exist(errs)) return false;
    if(!source.read(errs)) return false;
    this->sources.push_back(std::make_unique<Source>(std::move(source)));

    return true;
}

bool SourceManager::contain(const char* sourcePath)
{
    return std::any_of(this->sources.begin(), this->sources.end(), [&](const auto& source) {
        return source->getPath() == sourcePath;
    });
}

Source* SourceManager::getLastSource() { return this->sources.back().get(); }

bool SourceManager::hasSource() { return !this->sources.empty(); }

} // namespace Aerell