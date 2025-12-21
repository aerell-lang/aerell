#pragma once

#include <filesystem>

#include <llvm/Support/raw_ostream.h>

namespace Aerell
{

class Source
{
  public:
    Source(const char* path);

    bool exist(llvm::raw_ostream& errs);
    bool read(llvm::raw_ostream& errs);

    const std::string& getContent();
    const char* getContentData();
    std::string getPath() const;

    void printErrorMessage(size_t offset, size_t size, const char* msg);

  private:
    std::filesystem::path path;
    std::string content;
    std::vector<size_t> lineStarts;
};

} // namespace Aerell