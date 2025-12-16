#pragma once

#include <filesystem>

#include <llvm/Support/raw_ostream.h>

namespace Aerell
{

class Source
{
  public:
    Source(const char* path);

    bool exist();
    bool read();

    const std::string& getContent();
    std::string getPath() const;

    void printErrorMessage(size_t offset, size_t size, const char* msg);

  private:
    std::filesystem::path path;
    std::string content;
    std::vector<size_t> lineStarts;
};

} // namespace Aerell