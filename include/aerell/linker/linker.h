#pragma once

#include <vector>

namespace aerell
{

class Linker
{
  public:
    static void initialize();
    static bool linking(const char* name);

  private:
    static std::vector<std::string> libPathFlags;
};

} // namespace aerell