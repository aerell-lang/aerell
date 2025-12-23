#pragma once

#include <vector>

namespace Aerell
{

class Linker
{
  public:
    Linker();
    bool linking(std::string_view filePath);
    bool linking(const std::vector<std::string>& filePaths);

  private:
    static std::vector<std::string> libPathFlags;
    static bool findLibPathFlagsInit;
};

} // namespace Aerell