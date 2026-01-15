// Copyright 2026 Fern Aerell.
// SPDX-License-Identifier: Apache-2.0

#pragma once

#include <string>
#include <unordered_map>

#include "aerell/module/label.hpp"

namespace aerell
{

class Module
{
  public:
    Module();

    void setName(std::string_view name);

    std::string toStr() const;

    Label& addLabel(std::string_view name, bool pub);
    const Label& getLabel(std::string_view name);

  private:
    struct string_hash
    {
        using is_transparent = void;
        size_t operator()(std::string_view sv) const { return std::hash<std::string_view>{}(sv); }
    };

    std::string name;
    std::unordered_map<std::string, Label, string_hash, std::equal_to<>> labels;
};

} // namespace aerell