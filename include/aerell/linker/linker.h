#pragma once

#include <vector>

namespace Aerell
{

class Linker
{
  public:
    static void initialize();
    static bool linking(const char* name);

  private:
    static std::vector<std::string> libPathFlags;
};

} // namespace Aerell